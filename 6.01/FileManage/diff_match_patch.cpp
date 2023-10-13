#include "stdafx.h"


/*
 * Copyright 2008 Google Inc. All Rights Reserved.
 * Author: fraser@google.com (Neil Fraser)
 * Author: mikeslemmer@gmail.com (Mike Slemmer)
 * Author: snhere@gmail.com (Sergey Nozhenko)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Diff Match and Patch
 * http://code.google.com/p/google-diff-match-patch/
 */

#include <algorithm>
#include <cwctype>
#include <limits>
#include "diff_match_patch.h"

using std::wstring;

// Several helper functions

static wstring to_string(int n)
{
  wstring str;
  bool negative = false;
  size_t l = 0;
  if (n < 0) n = -n, ++l, negative = true;
  int n_ = n; do { ++l; } while ((n_ /= 10) > 0);
  str.resize(l);
  wstring::iterator s = str.end();
  const wchar_t digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
  do { *--s = digits[n % 10]; } while ((n /= 10) > 0);
  if (negative) *--s = '-';
  return str;
}

inline int to_int(const wstring& str)
{
  int n = 0;
  for (wstring::const_pointer s = str.c_str(); s != str.c_str() + str.size(); ++s) {
    int i;
    switch (*s) {
      case '0': i = 0; break;
      case '1': i = 1; break;
      case '2': i = 2; break;
      case '3': i = 3; break;
      case '4': i = 4; break;
      case '5': i = 5; break;
      case '6': i = 6; break;
      case '7': i = 7; break;
      case '8': i = 8; break;
      case '9': i = 9; break;
      default: return n;
    }
    n = n * 10 + i;
  }
  return n;
}

static void replace_all(wstring& str, wchar_t oldval, const wchar_t* newval)
{
  size_t n = wstring::traits_type::length(newval), i = 0;
  while ((i = str.find(oldval, i)) != wstring::npos) {
    str.replace(i, 1, newval, n);
    i += n;
  }
}

wstring::size_type next_token(const wstring& str, wchar_t delim, wstring::const_pointer off)
{
  wstring::const_pointer p = off, end = str.c_str() + str.length();
  for (; p != end; ++p) if (*p == delim) break;
  return p - off;
}

const unsigned UTF16_SURROGATE_MIN = 0xD800u, UTF16_SURROGATE_MAX = 0xDFFFu, UTF16_HIGH_SURROGATE_MAX = 0xDBFFu, UTF16_LOW_SURROGATE_MIN = 0xDC00u;
const unsigned UTF16_SURROGATE_OFFSET = (UTF16_SURROGATE_MIN << 10) + UTF16_HIGH_SURROGATE_MAX - 0xFFFFu;

static void append_percent_encoded(wstring& s1, const wstring& s2)
{
  const char safe_chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_.~ !*'();/?:@&=+$,#";

  bool safe[0x100];
  size_t i;
  for (i = 0; i < 0x100; ++i) safe[i] = false;
  for (i = 0; i < sizeof(safe_chars) / sizeof(char); ++i) safe[safe_chars[i]] = true;

  int n = 0;
  wstring::const_pointer c = s2.c_str(), ct = c + s2.length();
  for (; c != ct; ++c) n += *c >= UTF16_SURROGATE_MIN? 12 : *c >= 0x800? 9 : *c >= 0x80? 6 : safe[(unsigned char)*c]? 1 : 3;
  if (n == s2.length())
    s1.append(s2);
  else {
    s1.reserve(s1.size() + n);
    // Encode as UTF-8, then escape unsafe characters
    unsigned char utf8[4];
    for (c = s2.c_str(); c != ct; ++c) {
      unsigned u = *c;
      if (sizeof(wchar_t) < 4 && UTF16_SURROGATE_MIN <= u && u <= UTF16_HIGH_SURROGATE_MAX && c + 1 != ct)
        u = (u << 10) + *++c - UTF16_SURROGATE_OFFSET; // Assume it is a UTF-16 surrogate pair
      unsigned char* pt = utf8;
      if (u < 0x80)
        *pt++ = (unsigned char)u;  
      else if (u < 0x800) {
        *pt++ = (unsigned char)((u >> 6) | 0xC0);
        *pt++ = (unsigned char)((u & 0x3F) | 0x80);
      }
      else if (u < 0x10000) {
        *pt++ = (unsigned char)((u >> 12) | 0xE0);
        *pt++ = (unsigned char)(((u >> 6) & 0x3F) | 0x80);
        *pt++ = (unsigned char)((u & 0x3F) | 0x80);
      }
      else {
        *pt++ = (unsigned char)((u >> 18) | 0xF0);
        *pt++ = (unsigned char)(((u >> 12) & 0x3F) | 0x80);
        *pt++ = (unsigned char)(((u >> 6) & 0x3F) | 0x80);
        *pt++ = (unsigned char)((u & 0x3F) | 0x80);
      }

      for (const unsigned char* p = utf8; p < pt; ++p)
        if (safe[*p])
          s1 += (wchar_t)(char)*p;
        else {
          s1 += L'%';
          s1 += (wchar_t)safe_chars[(*p & 0xF0) >> 4];
          s1 += (wchar_t)safe_chars[*p & 0xF];
        }
    }
  }
}

static unsigned hex_digit_value(wchar_t c)
{
  switch (c)
  {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'A': case 'a': return 0xA;
    case 'B': case 'b': return 0xB;
    case 'C': case 'c': return 0xC;
    case 'D': case 'd': return 0xD;
    case 'E': case 'e': return 0xE;
    case 'F': case 'f': return 0xF;
  }
  throw wstring(L"Invalid character: ") + c;
}

static void percent_decode(wstring& str)
{
  wstring::iterator s2 = str.begin(), s3 = s2, s4 = s2;
  for (wstring::const_pointer s1 = str.c_str(), end = s1 + str.size(); s1 != end; ++s1, ++s2)
    if (*s1 != '%')
      *s2 = *s1;
    else {
      wchar_t d1 = *++s1;
      *s2 = wchar_t((hex_digit_value(d1) << 4) + hex_digit_value(*++s1));
    }
  // Decode UTF-8 string in-place
  while (s3 != s2) {
    unsigned u = *s3;
    switch (u < 0x80? 1 : ((u >> 5) == 6)? 2 : ((u >> 4) == 0xE)? 3 : ((u >> 3) == 0x1E)? 4 : 0) {
      case 1:
        break;
      case 2:
        if (++s3 == s2 || (*s3 & 0xC0) != 0x80) continue;
        u = ((u & 0x1F) << 6) + (*s3 & 0x3F);
        break;
      case 3:
        if (++s3 == s2 || (*s3 & 0xC0) != 0x80) continue;
        u = ((u & 0xF) << 12) + ((*s3 & 0x3F) << 6);
        if (++s3 == s2 || (*s3 & 0xC0) != 0x80) continue;
        u += *s3 & 0x3F;
        break;
      case 4:
        if (++s3 == s2 || (*s3 & 0xC0) != 0x80) continue;
        u = ((u & 7) << 18) + ((*s3 & 0x3F) << 12);
        if (++s3 == s2 || (*s3 & 0xC0) != 0x80) continue;
        u += (*s3 & 0x3F) << 6;
        if (++s3 == s2 || (*s3 & 0xC0) != 0x80) continue;
        u += *s3 & 0x3F; 
        break;
      default:
        ++s3;
        continue;
    }
    if (sizeof(wchar_t) < 4 && u > 0xFFFF) { // Encode code points that do not fit in wchar_t as UTF-16 surrogate pairs
      *s4++ = wchar_t((u >> 10) + UTF16_SURROGATE_MIN - (0x10000 >> 10));
      *s4++ = wchar_t((u & 0x3FF) + UTF16_LOW_SURROGATE_MIN);
    }
    else
      *s4++ = wchar_t(u);
    ++s3;
  }
  if (s4 != str.end()) str.resize(s4 - str.begin());
}

inline wstring right(const wstring& str, wstring::size_type n) { return str.substr(str.size() - n); }

inline bool is_control(wchar_t c) { return c == '\n' || c == '\r'; }



//////////////////////////
//
// Diff Class
//
//////////////////////////


wstring Diff::strOperation(Operation op) {
  switch (op) {
    case INSERT:
      return L"INSERT";
    case DELETEA:
      return L"DELETE";
    case EQUAL:
      return L"EQUAL";
  }
  throw "Invalid operation.";
}

/**
 * Display a human-readable version of this Diff.
 * @return text version
 */
wstring Diff::toString() const {
  wstring prettyText = text;
  // Replace linebreaks with Pilcrow signs.
  std::replace(prettyText.begin(), prettyText.end(), L'\n', L'\u00b6');
  return L"Diff(" + strOperation(operation) + L",\""
      + prettyText + L"\")";
}


/////////////////////////////////////////////
//
// Patch Class
//
/////////////////////////////////////////////


/**
 * Emmulate GNU diff's format.
 * Header: @@ -382,8 +481,9 @@
 * Indicies are printed as 1-based, not 0-based.
 * @return The GNU diff string
 */
wstring Patch::toString() const {
  wstring coords1, coords2;
  if (length1 == 0) {
    coords1 = to_string(start1) + L",0";
  } else if (length1 == 1) {
    coords1 = to_string(start1 + 1);
  } else {
    coords1 = to_string(start1 + 1) + L',' + to_string(length1);
  }
  if (length2 == 0) {
    coords2 = to_string(start2) + L",0";
  } else if (length2 == 1) {
    coords2 = to_string(start2 + 1);
  } else {
    coords2 = to_string(start2 + 1) + L',' + to_string(length2);
  }
  wstring text(L"@@ -" + coords1 + L" +" + coords2 + L" @@\n");
  // Escape the body of the patch with %xx notation.
  for (Diffs::const_iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
    switch ((*cur_diff).operation) {
      case INSERT:
        text += L'+';
        break;
      case DELETEA:
        text += L'-';
        break;
      case EQUAL:
        text += L' ';
        break;
    }
    append_percent_encoded(text, (*cur_diff).text);
    text += L'\n';
  }

  return text;
}


/////////////////////////////////////////////
//
// diff_match_patch Class
//
/////////////////////////////////////////////

diff_match_patch::diff_match_patch() :
  Diff_Timeout(1.0f),
  Diff_EditCost(4),
  Match_Threshold(0.5f),
  Match_Distance(1000),
  Patch_DeleteThreshold(0.5f),
  Patch_Margin(4),
  Match_MaxBits(32) {
}


Diffs diff_match_patch::diff_main(const wstring &text1,
                                        const wstring &text2) const {
  return diff_main(text1, text2, true);
}

Diffs diff_match_patch::diff_main(const wstring &text1,
    const wstring &text2, bool checklines) const {
  // Set a deadline by which time the diff must be complete.
  clock_t deadline;
  if (Diff_Timeout <= 0) {
	  deadline = (std::numeric_limits<clock_t>::max)();
  } else {
    deadline = clock() + (clock_t)(Diff_Timeout * CLOCKS_PER_SEC);
  }
  return diff_main(text1, text2, checklines, deadline);
}

Diffs diff_match_patch::diff_main(const wstring &text1,
    const wstring &text2, bool checklines, clock_t deadline) {

  // Check for equality (speedup).
  Diffs diffs;
  if (text1 == text2) {
    if (!text1.empty()) {
      diffs.push_back(Diff(EQUAL, text1));
    }
  }
  else {
    // Trim off common prefix (speedup).
    int commonlength = diff_commonPrefix(text1, text2);
    const wstring &commonprefix = text1.substr(0, commonlength);
    wstring textChopped1 = text1.substr(commonlength);
    wstring textChopped2 = text2.substr(commonlength);

    // Trim off common suffix (speedup).
    commonlength = diff_commonSuffix(textChopped1, textChopped2);
    const wstring &commonsuffix = right(textChopped1, commonlength);
    textChopped1 = textChopped1.substr(0, textChopped1.length() - commonlength);
    textChopped2 = textChopped2.substr(0, textChopped2.length() - commonlength);

    // Compute the diff on the middle block.
    diffs = diff_compute(textChopped1, textChopped2, checklines, deadline);

    // Restore the prefix and suffix.
    if (!commonprefix.empty()) {
      diffs.push_front(Diff(EQUAL, commonprefix));
    }
    if (!commonsuffix.empty()) {
      diffs.push_back(Diff(EQUAL, commonsuffix));
    }

    diff_cleanupMerge(diffs);
  }

  return diffs;
}


Diffs diff_match_patch::diff_compute(wstring text1, wstring text2,
    bool checklines, clock_t deadline) {
  Diffs diffs;

  do {
    if (text1.empty()) {
      // Just add some text (speedup).
      diffs.push_back(Diff(INSERT, text2));
      break; // return
    }

    if (text2.empty()) {
      // Just delete some text (speedup).
      diffs.push_back(Diff(DELETEA, text1));
      break; // return
    }

    {
      const wstring& longtext = text1.length() > text2.length() ? text1 : text2;
      const wstring& shorttext = text1.length() > text2.length() ? text2 : text1;
      const int i = longtext.find(shorttext);
      if (i != -1) {
        // Shorter text is inside the longer text (speedup).
        Operation op =  (text1.length() > text2.length()) ? DELETEA : INSERT;
        diffs.push_back(Diff(op, longtext.substr(0, i)));
        diffs.push_back(Diff(EQUAL, shorttext));
        diffs.push_back(Diff(op, safeMid(longtext, i + shorttext.length())));
        break; // return
      }

      if (shorttext.length() == 1) {
        // Single character string.
        // After the previous speedup, the character can't be an equality.
        diffs.push_back(Diff(DELETEA, text1));
        diffs.push_back(Diff(INSERT, text2));
        break; // return
      }
      // Garbage collect longtext and shorttext by scoping out.
    }

    // Don't risk returning a non-optimal diff if we have unlimited time.
    if (deadline != (std::numeric_limits<clock_t>::max)()) {
      // Check to see if the problem can be split in two.
      HalfMatchResult hm;
      if (diff_halfMatch(text1, text2, hm)) {
        // A half-match was found, sort out the return data.
        // Send both pairs off for separate processing.
        diffs = diff_main(hm.text1_a, hm.text2_a, checklines, deadline);
        diffs.push_back(Diff(EQUAL, hm.mid_common));
        Diffs diffs_b = diff_main(hm.text1_b, hm.text2_b, checklines, deadline);
        diffs.splice(diffs.end(), diffs_b);
        break; // return
      }
    }

    // Perform a real diff.
    if (checklines && (text1.length() < 100 || text2.length() < 100)) {
      checklines = false;  // Too trivial for the overhead.
    }
    Lines linearray;
    if (checklines) {
      // Scan the text on a line-by-line basis first.
      diff_linesToChars(text1, text2, linearray);
    }

    diffs = diff_bisect(text1, text2, deadline);

    if (checklines) {
      // Convert the diff back to original text.
      diff_charsToLines(diffs, linearray);
      // Eliminate freak matches (e.g. blank lines)
      diff_cleanupSemantic(diffs);

      // Rediff any replacement blocks, this time character-by-character.
      // Add a dummy entry at the end.
      diffs.push_back(Diff(EQUAL, wstring()));
      int count_delete = 0;
      int count_insert = 0;
      wstring text_delete;
      wstring text_insert;

      for (Diffs::iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
        switch ((*cur_diff).operation) {
          case INSERT:
            count_insert++;
            text_insert += (*cur_diff).text;
            break;
          case DELETEA:
            count_delete++;
            text_delete += (*cur_diff).text;
            break;
          case EQUAL:
            // Upon reaching an equality, check for prior redundancies.
            if (count_delete >= 1 && count_insert >= 1) {
              // Delete the offending records and add the merged ones.
              Diffs::iterator last = cur_diff;
              std::advance(cur_diff, -(count_delete + count_insert));
              cur_diff = diffs.erase(cur_diff, last);

              Diffs new_diffs = diff_main(text_delete, text_insert, false, deadline);
              diffs.splice(cur_diff++, new_diffs);
              --cur_diff;
            }
            count_insert = 0;
            count_delete = 0;
            text_delete.clear();
            text_insert.clear();
            break;
        }
      }
      diffs.pop_back();  // Remove the dummy entry at the end.
    }
  } while (false);
  return diffs;
}


Diffs diff_match_patch::diff_bisect(const wstring &text1,
    const wstring &text2, clock_t deadline) {
  // Cache the text lengths to prevent multiple calls.
  const int text1_length = text1.length();
  const int text2_length = text2.length();
  const int max_d = (text1_length + text2_length + 1) / 2;
  const int v_offset = max_d;
  const int v_length = 2 * max_d;
  int *v1 = new int[v_length];
  int *v2 = new int[v_length];
  for (int x = 0; x < v_length; x++) {
    v1[x] = -1;
    v2[x] = -1;
  }
  v1[v_offset + 1] = 0;
  v2[v_offset + 1] = 0;
  const int delta = text1_length - text2_length;
  // If the total number of characters is odd, then the front path will
  // collide with the reverse path.
  const bool front = (delta % 2 != 0);
  // Offsets for start and end of k loop.
  // Prevents mapping of space beyond the grid.
  int k1start = 0;
  int k1end = 0;
  int k2start = 0;
  int k2end = 0;
  for (int d = 0; d < max_d; d++) {
    // Bail out if deadline is reached.
    if (clock() > deadline) {
      break;
    }

    // Walk the front path one step.
    for (int k1 = -d + k1start; k1 <= d - k1end; k1 += 2) {
      const int k1_offset = v_offset + k1;
      int x1;
      if (k1 == -d || (k1 != d && v1[k1_offset - 1] < v1[k1_offset + 1])) {
        x1 = v1[k1_offset + 1];
      } else {
        x1 = v1[k1_offset - 1] + 1;
      }
      int y1 = x1 - k1;
      while (x1 < text1_length && y1 < text2_length
          && text1[x1] == text2[y1]) {
        x1++;
        y1++;
      }
      v1[k1_offset] = x1;
      if (x1 > text1_length) {
        // Ran off the right of the graph.
        k1end += 2;
      } else if (y1 > text2_length) {
        // Ran off the bottom of the graph.
        k1start += 2;
      } else if (front) {
        int k2_offset = v_offset + delta - k1;
        if (k2_offset >= 0 && k2_offset < v_length && v2[k2_offset] != -1) {
          // Mirror x2 onto top-left coordinate system.
          int x2 = text1_length - v2[k2_offset];
          if (x1 >= x2) {
            // Overlap detected.
            return diff_bisectSplit(text1, text2, x1, y1, deadline);
          }
        }
      }
    }

    // Walk the reverse path one step.
    for (int k2 = -d + k2start; k2 <= d - k2end; k2 += 2) {
      const int k2_offset = v_offset + k2;
      int x2;
      if (k2 == -d || (k2 != d && v2[k2_offset - 1] < v2[k2_offset + 1])) {
        x2 = v2[k2_offset + 1];
      } else {
        x2 = v2[k2_offset - 1] + 1;
      }
      int y2 = x2 - k2;
      while (x2 < text1_length && y2 < text2_length
          && text1[text1_length - x2 - 1] == text2[text2_length - y2 - 1]) {
        x2++;
        y2++;
      }
      v2[k2_offset] = x2;
      if (x2 > text1_length) {
        // Ran off the left of the graph.
        k2end += 2;
      } else if (y2 > text2_length) {
        // Ran off the top of the graph.
        k2start += 2;
      } else if (!front) {
        int k1_offset = v_offset + delta - k2;
        if (k1_offset >= 0 && k1_offset < v_length && v1[k1_offset] != -1) {
          int x1 = v1[k1_offset];
          int y1 = v_offset + x1 - k1_offset;
          // Mirror x2 onto top-left coordinate system.
          x2 = text1_length - x2;
          if (x1 >= x2) {
            // Overlap detected.
            return diff_bisectSplit(text1, text2, x1, y1, deadline);
          }
        }
      }
    }
  }
  // Diff took too long and hit the deadline or
  // number of diffs equals number of characters, no commonality at all.
  Diffs diffs;
  diffs.push_back(Diff(DELETEA, text1));
  diffs.push_back(Diff(INSERT, text2));
  return diffs;
}

Diffs diff_match_patch::diff_bisectSplit(const wstring &text1,
    const wstring &text2, int x, int y, clock_t deadline) {
  wstring text1a = text1.substr(0, x);
  wstring text2a = text2.substr(0, y);
  wstring text1b = safeMid(text1, x);
  wstring text2b = safeMid(text2, y);

  // Compute both diffs serially.
  Diffs diffs_a = diff_main(text1a, text2a, false, deadline);
  Diffs diffs_b = diff_main(text1b, text2b, false, deadline);
  diffs_a.splice(diffs_a.end(), diffs_b);
  return diffs_a;
}

void diff_match_patch::diff_linesToChars(wstring &text1, wstring &text2, Lines& lineArray)
{
  std::map<LinePtr, size_t> lineHash;
  lineArray.text1.swap(text1), lineArray.text2.swap(text2);
  // e.g. linearray[4] == "Hello\n"
  // e.g. linehash.get("Hello\n") == 4

  // "\x00" is a valid character, but various debuggers don't like it.
  // So we'll insert a junk entry to avoid generating a null character.

  text1 = diff_linesToCharsMunge(lineArray.text1, lineHash);
  text2 = diff_linesToCharsMunge(lineArray.text2, lineHash);

  lineArray.resize(lineHash.size() + 1);
  for (std::map<LinePtr, size_t>::const_iterator i = lineHash.begin(); i != lineHash.end(); ++i)
    lineArray[(*i).second] = (*i).first;
}


wstring diff_match_patch::diff_linesToCharsMunge(const wstring &text,
                                                 std::map<LinePtr, size_t> &lineHash) {
  wstring chars;
  // Walk the text, pulling out a substring for each line.
  // text.split('\n') would would temporarily double our memory footprint.
  // Modifying text would create many large strings to garbage collect.
  wstring::size_type lineLen;
  for (wstring::const_pointer lineStart = text.c_str(), textEnd = lineStart + text.size(); lineStart < textEnd; lineStart += lineLen + 1) {
    lineLen = next_token(text, L'\n', lineStart);
    if (lineStart + lineLen == textEnd) --lineLen;
    chars += (wchar_t)(*lineHash.insert(std::make_pair(LinePtr(lineStart, lineLen + 1), (int)lineHash.size() + 1)).first).second;
  }
  return chars;
}


void diff_match_patch::diff_charsToLines(Diffs &diffs,
                                         const Lines &lineArray) {
  for (Diffs::iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
    wstring text;
    for (int y = 0; y < (int)(*cur_diff).text.length(); y++) {
      const LinePtr& lp = lineArray[static_cast<size_t>((*cur_diff).text[y])];
      text.append(lp.first, lp.second);
    }
    (*cur_diff).text.swap(text);
  }
}


int diff_match_patch::diff_commonPrefix(const wstring &text1,
                                        const wstring &text2) {
  // Performance analysis: http://neil.fraser.name/news/2007/10/09/
  const int n = (std::min)(text1.length(), text2.length());
  for (int i = 0; i < n; i++) {
    if (text1[i] != text2[i]) {
      return i;
    }
  }
  return n;
}


int diff_match_patch::diff_commonSuffix(const wstring &text1,
                                        const wstring &text2) {
  // Performance analysis: http://neil.fraser.name/news/2007/10/09/
  const int text1_length = text1.length();
  const int text2_length = text2.length();
  const int n = (std::min)(text1_length, text2_length);
  for (int i = 1; i <= n; i++) {
    if (text1[text1_length - i] != text2[text2_length - i]) {
      return i - 1;
    }
  }
  return n;
}

int diff_match_patch::diff_commonOverlap(const wstring &text1,
                                         const wstring &text2) {
  // Cache the text lengths to prevent multiple calls.
  const int text1_length = text1.length();
  const int text2_length = text2.length();
  // Eliminate the null case.
  if (text1_length == 0 || text2_length == 0) {
    return 0;
  }
  // Truncate the longer string.
  wstring text1_trunc = text1;
  wstring text2_trunc = text2;
  if (text1_length > text2_length) {
    text1_trunc = right(text1, text2_length);
  } else if (text1_length < text2_length) {
    text2_trunc = text2.substr(0, text1_length);
  }
  const int text_length = (std::min)(text1_length, text2_length);
  // Quick check for the worst case.
  if (text1_trunc == text2_trunc) {
    return text_length;
  }

  // Start by looking for a single character match
  // and increase length until no match is found.
  // Performance analysis: http://neil.fraser.name/news/2010/11/04/
  int best = 0;
  int length = 1;
  while (true) {
    wstring pattern = right(text1_trunc, length);
    int found = text2_trunc.find(pattern);
    if (found == -1) {
      return best;
    }
    length += found;
    if (found == 0 || right(text1_trunc, length) == right(text2_trunc, length)) {
      best = length;
      length++;
    }
  }
}

bool diff_match_patch::diff_halfMatch(const wstring &text1, const wstring &text2,
                                                              HalfMatchResult& hm) {
  const wstring longtext = text1.length() > text2.length() ? text1 : text2;
  const wstring shorttext = text1.length() > text2.length() ? text2 : text1;
  if (longtext.length() < 4 || shorttext.length() * 2 < longtext.length()) {
    return false;  // Pointless.
  }

  HalfMatchResult res1, res2;
  // First check if the second quarter is the seed for a half-match.
  bool hm1 = diff_halfMatchI(longtext, shorttext,
      (longtext.length() + 3) / 4, res1);
  // Check again based on the third quarter.
  bool hm2 = diff_halfMatchI(longtext, shorttext,
      (longtext.length() + 1) / 2, res2);
  if (!hm1 && !hm2) {
    return false;
  } else if (!hm2) {
    hm.swap(res1);
  } else if (!hm1) {
    hm.swap(res2);
  } else {
    // Both matched.  Select the longest.
    hm.swap(res1.mid_common.length() > res2.mid_common.length() ? res1 : res2);
  }

  // A half-match was found, sort out the return data.
  if (text1.length() <= text2.length()) {
    hm.text1_a.swap(hm.text2_a);
    hm.text1_b.swap(hm.text2_b);
  }
  return true;
}


bool diff_match_patch::diff_halfMatchI(const wstring &longtext,
                                              const wstring &shorttext,
                                              int i, HalfMatchResult& best) {
  // Start with a 1/4 length substring at position i as a seed.
  const wstring seed = safeMid(longtext, i, longtext.length() / 4);
  int j = -1;
  while ((j = shorttext.find(seed, j + 1)) != -1) {
    const int prefixLength = diff_commonPrefix(safeMid(longtext, i),
        safeMid(shorttext, j));
    const int suffixLength = diff_commonSuffix(longtext.substr(0, i),
        shorttext.substr(0, j));
    if ((int)best.mid_common.length() < suffixLength + prefixLength) {
      best.mid_common = safeMid(shorttext, j - suffixLength, suffixLength)
          + safeMid(shorttext, j, prefixLength);
      best.text1_a = longtext.substr(0, i - suffixLength);
      best.text1_b = safeMid(longtext, i + prefixLength);
      best.text2_a = shorttext.substr(0, j - suffixLength);
      best.text2_b = safeMid(shorttext, j + prefixLength);
    }
  }
  return best.mid_common.length() * 2 >= longtext.length();
}


void diff_match_patch::diff_cleanupSemantic(Diffs &diffs) {
  if (diffs.empty()) {
    return;
  }
  bool changes = false;
  std::vector<Diffs::iterator> equalities;  // Stack of equalities.
  wstring lastequality;  // Always equal to equalities.lastElement().text
  Diffs::iterator cur_diff;
  // Number of characters that changed prior to the equality.
  int length_insertions1 = 0;
  int length_deletions1 = 0;
  // Number of characters that changed after the equality.
  int length_insertions2 = 0;
  int length_deletions2 = 0;
  for (cur_diff = diffs.begin(); cur_diff != diffs.end();) {
    if ((*cur_diff).operation == EQUAL) {
      // Equality found.
      equalities.push_back(cur_diff);
      length_insertions1 = length_insertions2;
      length_deletions1 = length_deletions2;
      length_insertions2 = 0;
      length_deletions2 = 0;
      lastequality = (*cur_diff).text;
    } else {
      // An insertion or deletion.
      if ((*cur_diff).operation == INSERT) {
        length_insertions2 += (*cur_diff).text.length();
      } else {
        length_deletions2 += (*cur_diff).text.length();
      }
      if (!lastequality.empty()
          && ((int)lastequality.length()
              <= (std::max)(length_insertions1, length_deletions1))
          && ((int)lastequality.length()
              <= (std::max)(length_insertions2, length_deletions2))) {
        // printf("Splitting: '%s'\n", qPrintable(lastequality));
        // Walk back to offending equality.
        // Change second copy to insert.
        (*(cur_diff = equalities.back())).operation = INSERT;
        // Duplicate record.
        diffs.insert(cur_diff, Diff(DELETEA, lastequality));
        equalities.pop_back();  // Throw away the equality we just deleted.
        if (!equalities.empty()) {
          // Throw away the previous equality (it needs to be reevaluated).
          equalities.pop_back();
        }
        length_insertions1 = 0;  // Reset the counters.
        length_deletions1 = 0;
        length_insertions2 = 0;
        length_deletions2 = 0;
        lastequality = wstring();
        changes = true;

        if (!equalities.empty())
          // There is a safe equality we can fall back to.
          cur_diff = equalities.back();
        else
        {
          // There are no previous equalities, walk back to the start.
          cur_diff = diffs.begin();
          continue;
        }
      }
    }
    ++cur_diff;
  }

  // Normalize the diff.
  if (changes) {
    diff_cleanupMerge(diffs);
  }
  diff_cleanupSemanticLossless(diffs);

  // Find any overlaps between deletions and insertions.
  // e.g: <del>abcxx</del><ins>xxdef</ins>
  //   -> <del>abc</del>xx<ins>def</ins>
  if ((cur_diff = diffs.begin()) != diffs.end()) {
    for (Diffs::iterator prev_diff = cur_diff; ++cur_diff != diffs.end(); prev_diff = cur_diff) {
      if ((*prev_diff).operation == DELETEA &&
          (*cur_diff).operation == INSERT) {
        wstring deletion = (*prev_diff).text;
        wstring insertion = (*cur_diff).text;
        int overlap_length = diff_commonOverlap(deletion, insertion);
        if (overlap_length != 0) {
          // Overlap found.  Insert an equality and trim the surrounding edits.
          diffs.insert(cur_diff, Diff(EQUAL, insertion.substr(0, overlap_length)));
          (*prev_diff).text =
              deletion.substr(0, deletion.length() - overlap_length);
          (*cur_diff).text = safeMid(insertion, overlap_length);
          // diffs.insert inserts the element before the cursor, so there is
          // no need to step past the new element.
        }
        if (++cur_diff == diffs.end()) break;
      }
    }
  }
}


void diff_match_patch::diff_cleanupSemanticLossless(Diffs &diffs) {
  wstring equality1, edit, equality2;
  wstring commonString;
  int commonOffset;
  int score, bestScore;
  wstring bestEquality1, bestEdit, bestEquality2;
  // Create a new iterator at the start.
  Diffs::iterator prev_diff = diffs.begin(), cur_diff = prev_diff;
  if (prev_diff == diffs.end() || ++cur_diff == diffs.end()) return;

  // Intentionally ignore the first and last element (don't need checking).
  for (Diffs::iterator next_diff = cur_diff; ++next_diff != diffs.end(); prev_diff = cur_diff, cur_diff = next_diff) {
    if ((*prev_diff).operation == EQUAL &&
      (*next_diff).operation == EQUAL) {
        // This is a single edit surrounded by equalities.
        equality1 = (*prev_diff).text;
        edit = (*cur_diff).text;
        equality2 = (*next_diff).text;

        // First, shift the edit as far left as possible.
        commonOffset = diff_commonSuffix(equality1, edit);
        if (commonOffset != 0) {
          commonString = safeMid(edit, edit.length() - commonOffset);
          equality1 = equality1.substr(0, equality1.length() - commonOffset);
          edit = commonString + edit.substr(0, edit.length() - commonOffset);
          equality2 = commonString + equality2;
        }

        // Second, step character by character right, looking for the best fit.
        bestEquality1 = equality1;
        bestEdit = edit;
        bestEquality2 = equality2;
        bestScore = diff_cleanupSemanticScore(equality1, edit)
            + diff_cleanupSemanticScore(edit, equality2);
        while (!edit.empty() && !equality2.empty()
            && edit[0] == equality2[0]) {
          equality1 += edit[0];
          edit = safeMid(edit, 1) + equality2[0];
          equality2 = safeMid(equality2, 1);
          score = diff_cleanupSemanticScore(equality1, edit)
              + diff_cleanupSemanticScore(edit, equality2);
          // The >= encourages trailing rather than leading whitespace on edits.
          if (score >= bestScore) {
            bestScore = score;
            bestEquality1 = equality1;
            bestEdit = edit;
            bestEquality2 = equality2;
          }
        }

        if ((*prev_diff).text != bestEquality1) {
          // We have an improvement, save it back to the diff.
          if (!bestEquality1.empty()) {
            (*prev_diff).text = bestEquality1;
          } else {
            diffs.erase(prev_diff);
          }
          (*cur_diff).text = bestEdit;
          if (!bestEquality2.empty()) {
            (*next_diff).text = bestEquality2;
          } else {
            diffs.erase(next_diff); // Delete nextDiff.
            next_diff = cur_diff;
            cur_diff = prev_diff;
          }
        }
    }
  }
}


int diff_match_patch::diff_cleanupSemanticScore(const wstring &one,
                                                const wstring &two) {
  if (one.empty() || two.empty()) {
    // Edges are the best.
    return 10;
  }

  // Each port of this function behaves slightly differently due to
  // subtle differences in each language's definition of things like
  // 'whitespace'.  Since this function's purpose is largely cosmetic,
  // the choice has been made to use each language's native features
  // rather than force total conformity.
  int score = 0;
  // One point for non-alphanumeric.
  if (!std::iswalnum(one[one.length() - 1]) || !std::iswalnum(two[0])) {
    score++;
    // Two points for whitespace.
    if (std::iswspace(one[one.length() - 1]) || std::iswspace(two[0])) {
      score++;
      // Three points for line breaks.

      wstring::const_pointer p1 = one.c_str() + one.length() - 1, p2 = two.c_str();
      if (is_control(*p1) || is_control(*p2)) {
        score++;
        // Four points for blank lines.
        if (*p1 == '\n' && p1 != one.c_str() && (*(p1 - 1) == '\n' || *(p1 - 1) == '\r' && p1 - 1 != one.c_str() && *(p1 - 2) == '\n')) {
          score++;
        }
        else {
          p1 = p2 + two.length();
          if (*p2 == L'\r') ++p2;
          if (p2 != p1 && *p2 == '\n') {
            if (++p2 != p1 && *p2 == '\r') ++p2;
            if (p2 != p1 && *p2 == '\n') score++;
          }
        }
      }
    }
  }
  return score;
}


void diff_match_patch::diff_cleanupEfficiency(Diffs &diffs) const {
  if (diffs.empty()) {
    return;
  }
  bool changes = false;
  std::vector<Diffs::iterator> equalities;  // Stack of equalities.
  wstring lastequality;  // Always equal to equalities.lastElement().text
  // Is there an insertion operation before the last equality.
  bool pre_ins = false;
  // Is there a deletion operation before the last equality.
  bool pre_del = false;
  // Is there an insertion operation after the last equality.
  bool post_ins = false;
  // Is there a deletion operation after the last equality.
  bool post_del = false;

  for (Diffs::iterator cur_diff = diffs.begin(); cur_diff != diffs.end();) {
    if ((*cur_diff).operation == EQUAL) {
      // Equality found.
      if ((int)(*cur_diff).text.length() < Diff_EditCost && (post_ins || post_del)) {
        // Candidate found.
        equalities.push_back(cur_diff);
        pre_ins = post_ins;
        pre_del = post_del;
        lastequality = (*cur_diff).text;
      } else {
        // Not a candidate, and can never become one.
        equalities.clear();
        lastequality.clear();
      }
      post_ins = post_del = false;
    } else {
      // An insertion or deletion.
      if ((*cur_diff).operation == DELETEA) {
        post_del = true;
      } else {
        post_ins = true;
      }
      /*
      * Five types to be split:
      * <ins>A</ins><del>B</del>XY<ins>C</ins><del>D</del>
      * <ins>A</ins>X<ins>C</ins><del>D</del>
      * <ins>A</ins><del>B</del>X<ins>C</ins>
      * <ins>A</del>X<ins>C</ins><del>D</del>
      * <ins>A</ins><del>B</del>X<del>C</del>
      */
      if (!lastequality.empty()
          && ((pre_ins && pre_del && post_ins && post_del)
          || (((int)lastequality.length() < Diff_EditCost / 2)
          && ((pre_ins ? 1 : 0) + (pre_del ? 1 : 0)
          + (post_ins ? 1 : 0) + (post_del ? 1 : 0)) == 3))) {
        // printf("Splitting: '%s'\n", qPrintable(lastequality));
        // Walk back to offending equality.
        // Change second copy to insert.
        (*(cur_diff = equalities.back())).operation = INSERT;
        // Duplicate record.
        diffs.insert(cur_diff, Diff(DELETEA, lastequality));
        equalities.pop_back();  // Throw away the equality we just deleted.
        lastequality.clear();
        changes = true;
        if (pre_ins && pre_del) {
          // No changes made which could affect previous entry, keep going.
          post_ins = post_del = true;
          equalities.clear();
        } else {
          if (!equalities.empty()) {
            // Throw away the previous equality (it needs to be reevaluated).
            equalities.pop_back();
          }
          post_ins = post_del = false;
          if (!equalities.empty())
            // There is a safe equality we can fall back to.
            cur_diff = equalities.back();
          else
          {
            // There are no previous equalities, walk back to the start.
            cur_diff = diffs.begin();
            continue;
          }
        }
      }
    }
    ++cur_diff;
  }

  if (changes) {
    diff_cleanupMerge(diffs);
  }
}


void diff_match_patch::diff_cleanupMerge(Diffs &diffs) {
  diffs.push_back(Diff(EQUAL, wstring()));  // Add a dummy entry at the end.
  Diffs::iterator prev_diff, cur_diff;
  int count_delete = 0;
  int count_insert = 0;
  wstring text_delete;
  wstring text_insert;
  Diff *prevEqual = NULL;
  int commonlength;
  for (cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
    switch ((*cur_diff).operation) {
      case INSERT:
        count_insert++;
        text_insert += (*cur_diff).text;
        prevEqual = NULL;
        break;
      case DELETEA:
        count_delete++;
        text_delete += (*cur_diff).text;
        prevEqual = NULL;
        break;
      case EQUAL:
        if (count_delete + count_insert > 1) {
          // Delete the offending records.
          prev_diff = cur_diff;
          std::advance(prev_diff, -(count_delete + count_insert));
          diffs.erase(prev_diff, cur_diff);
          if (count_delete != 0 && count_insert != 0) {
            // Factor out any common prefixies.
            commonlength = diff_commonPrefix(text_insert, text_delete);
            if (commonlength != 0) {
              if (cur_diff != diffs.begin()) {
                prev_diff = cur_diff;
                if ((*--prev_diff).operation != EQUAL) {
                  throw "Previous diff should have been an equality.";
                }
                (*prev_diff).text += text_insert.substr(0, commonlength);
              } else {
                diffs.insert(cur_diff, Diff(EQUAL, text_insert.substr(0, commonlength)));
              }
              text_insert = safeMid(text_insert, commonlength);
              text_delete = safeMid(text_delete, commonlength);
            }
            // Factor out any common suffixies.
            commonlength = diff_commonSuffix(text_insert, text_delete);
            if (commonlength != 0) {
              (*cur_diff).text = safeMid(text_insert, text_insert.length()
                  - commonlength) + (*cur_diff).text;
              text_insert = text_insert.substr(0, text_insert.length()
                  - commonlength);
              text_delete = text_delete.substr(0, text_delete.length()
                  - commonlength);
            }
          }
          // Insert the merged records.
          if (!text_delete.empty()) {
            diffs.insert(cur_diff, Diff(DELETEA, text_delete));
          }
          if (!text_insert.empty()) {
            diffs.insert(cur_diff, Diff(INSERT, text_insert));
          }
        } else if (prevEqual != NULL) {
          // Merge this equality with the previous one.
          prevEqual->text += (*cur_diff).text;
          diffs.erase(cur_diff--);
        }
        
        count_insert = 0;
        count_delete = 0;
        text_delete.clear();
        text_insert.clear();
        prevEqual = &*cur_diff;
        break;
      }

  }
  if (diffs.back().text.empty()) {
    diffs.pop_back();  // Remove the dummy entry at the end.
  }

  /*
  * Second pass: look for single edits surrounded on both sides by equalities
  * which can be shifted sideways to eliminate an equality.
  * e.g: A<ins>BA</ins>C -> <ins>AB</ins>AC
  */
  bool changes = false;
  // Create a new iterator at the start.
  // (As opposed to walking the current one back.)
  prev_diff = cur_diff = diffs.begin();
  if (prev_diff != diffs.end() && ++cur_diff != diffs.end()) {
    // Intentionally ignore the first and last element (don't need checking).
    for (Diffs::iterator next_diff = cur_diff; ++next_diff != diffs.end(); prev_diff = cur_diff, cur_diff = next_diff) {
      if ((*prev_diff).operation == EQUAL &&
        (*next_diff).operation == EQUAL) {
          // This is a single edit surrounded by equalities.
          if ((*cur_diff).text.size() >= (*prev_diff).text.size() &&
              (*cur_diff).text.compare((*cur_diff).text.size() - (*prev_diff).text.size(), (*prev_diff).text.size(), (*prev_diff).text) == 0) {
            // Shift the edit over the previous equality.
            (*cur_diff).text = (*prev_diff).text
                + (*cur_diff).text.substr(0, (*cur_diff).text.length()
                - (*prev_diff).text.length());
            (*next_diff).text = (*prev_diff).text + (*next_diff).text;
            diffs.erase(prev_diff);
            cur_diff = next_diff;
            changes = true;
            if (++next_diff == diffs.end()) break;
          } else if ((*cur_diff).text.size() >= (*next_diff).text.size() && (*cur_diff).text.compare(0, (*next_diff).text.size(), (*next_diff).text) == 0) {
            // Shift the edit over the next equality.
            (*prev_diff).text += (*next_diff).text;
            (*cur_diff).text = safeMid((*cur_diff).text, (*next_diff).text.length())
                + (*next_diff).text;
            next_diff = diffs.erase(next_diff); // Delete nextDiff.
            changes = true;
            if (next_diff == diffs.end()) break;
          }
      }
    }
  }
  // If shifts were made, the diff needs reordering and another shift sweep.
  if (changes) {
    diff_cleanupMerge(diffs);
  }
}


int diff_match_patch::diff_xIndex(const Diffs &diffs, int loc) {
  int chars1 = 0;
  int chars2 = 0;
  int last_chars1 = 0;
  int last_chars2 = 0;
  Diffs::const_iterator last_diff = diffs.end();
  for (Diffs::const_iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
    if ((*cur_diff).operation != INSERT) {
      // Equality or deletion.
      chars1 += (*cur_diff).text.length();
    }
    if ((*cur_diff).operation != DELETEA) {
      // Equality or insertion.
      chars2 += (*cur_diff).text.length();
    }
    if (chars1 > loc) {
      // Overshot the location.
      last_diff = cur_diff;
      break;
    }
    last_chars1 = chars1;
    last_chars2 = chars2;
  }
  if (last_diff != diffs.end() && (*last_diff).operation == DELETE) {
    // The location was deleted.
    return last_chars2;
  }
  // Add the remaining character length.
  return last_chars2 + (loc - last_chars1);
}


wstring diff_match_patch::diff_prettyHtml(const Diffs &diffs) {
  wstring html;
  wstring text;
  int i = 0;
  for (Diffs::const_iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
    text = (*cur_diff).text;
    replace_all(text, '&', L"&amp;");
    replace_all(text, '<', L"&lt;");
    replace_all(text, '>', L"&gt;");
    replace_all(text, '\n', L"&para;<br>");
    switch ((*cur_diff).operation) {
      case INSERT:
        html += L"<ins style=\"background:#e6ffe6;\">" + text
            + L"</ins>";
        break;
      case DELETEA:
        html += L"<del style=\"background:#ffe6e6;\">" + text
            + L"</del>";
        break;
      case EQUAL:
        html += L"<span>" + text + L"</span>";
        break;
    }
    if ((*cur_diff).operation != DELETEA) {
      i += (*cur_diff).text.length();
    }
  }
  return html;
}


wstring diff_match_patch::diff_text1(const Diffs &diffs) {
  wstring text;
  for (Diffs::const_iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
    if ((*cur_diff).operation != INSERT) {
      text += (*cur_diff).text;
    }
  }
  return text;
}


wstring diff_match_patch::diff_text2(const Diffs &diffs) {
  wstring text;
  for (Diffs::const_iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
    if ((*cur_diff).operation != DELETEA) {
      text += (*cur_diff).text;
    }
  }
  return text;
}


int diff_match_patch::diff_levenshtein(const Diffs &diffs) {
  int levenshtein = 0;
  int insertions = 0;
  int deletions = 0;
  for (Diffs::const_iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
    switch ((*cur_diff).operation) {
      case INSERT:
        insertions += (*cur_diff).text.length();
        break;
      case DELETEA:
        deletions += (*cur_diff).text.length();
        break;
      case EQUAL:
        // A deletion and an insertion is one substitution.
        levenshtein += (std::max)(insertions, deletions);
        insertions = 0;
        deletions = 0;
        break;
    }
  }
  levenshtein += (std::max)(insertions, deletions);
  return levenshtein;
}


wstring diff_match_patch::diff_toDelta(const Diffs &diffs) {
  wstring text;
  for (Diffs::const_iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
    switch ((*cur_diff).operation) {
      case INSERT: {
        text += L'+';
        append_percent_encoded(text, (*cur_diff).text);
        text += L'\t';
        break;
      }
      case DELETEA:
        text += L'-' + (*cur_diff).text.length() + L'\t';
        break;
      case EQUAL:
        text += L'=' + (*cur_diff).text.length() + L'\t';
        break;
    }
  }
  if (!text.empty()) {
    // Strip off trailing tab character.
    text = text.substr(0, text.length() - 1);
  }
  return text;
}



Diffs diff_match_patch::diff_fromDelta(const wstring &text1,
                                             const wstring &delta) {
  Diffs diffs;
  int pointer = 0;  // Cursor in text1
  wstring::size_type token_len;
  for (wstring::const_pointer token = delta.c_str(); token - delta.c_str() < (int)delta.length(); token += token_len + 1) {
    token_len = next_token(delta, '\t', token);
    if (token_len == 0) {
      // Blank tokens are ok (from a trailing \t).
      continue;
    }
    // Each token begins with a one character parameter which specifies the
    // operation of this token (delete, insert, equality).
    wstring param(token + 1, token_len - 1);
    switch (*token) {
      case L'+':
        percent_decode(param);
        diffs.push_back(Diff(INSERT, param));
        break;
      case L'-':
        // Fall through.
      case L'=': {
        int n;
        n = to_int(param);
        if (n < 0) {
          throw L"Negative number in diff_fromDelta: " + param;
        }
        wstring text;
        text = safeMid(text1, pointer, n);
        pointer += n;
        if (*token == L'=') {
          diffs.push_back(Diff(EQUAL, text));
        } else {
          diffs.push_back(Diff(DELETEA, text));
        }
        break;
      }
      default:
        throw wstring(L"Invalid diff operation in diff_fromDelta: ") + *token;
    }
  }
  if (pointer != text1.length()) {
	  CString str; str.Format(L"%d",text1.length());
	  throw L"Delta length (" + to_string(pointer) + L") smaller than source text length (" + str.AllocSysString() + L')';
  }
  return diffs;
}



//  MATCH FUNCTIONS


int diff_match_patch::match_main(const wstring &text, const wstring &pattern,
                                 int loc) const {
  loc = (std::max)(0, (std::min)(loc, (int)text.length()));
  if (text == pattern) {
    // Shortcut (potentially not guaranteed by the algorithm)
    return 0;
  } else if (text.empty()) {
    // Nothing to match.
    return -1;
  } else if (loc + pattern.length() <= text.length()
      && safeMid(text, loc, pattern.length()) == pattern) {
    // Perfect match at the perfect spot!  (Includes case of null pattern)
    return loc;
  } else {
    // Do a fuzzy compare.
    return match_bitap(text, pattern, loc);
  }
}


int diff_match_patch::match_bitap(const wstring &text, const wstring &pattern,
                                  int loc) const {
  if (!(Match_MaxBits == 0 || (int)pattern.length() <= Match_MaxBits)) {
    throw "Pattern too long for this application.";
  }

  // Initialise the alphabet.
  std::map<wchar_t, int> s; 
  match_alphabet(pattern, s);

  // Highest score beyond which we give up.
  double score_threshold = Match_Threshold;
  // Is there a nearby exact match? (speedup)
  int best_loc = text.find(pattern, loc);
  if (best_loc != -1) {
    score_threshold = (std::min)(match_bitapScore(0, best_loc, loc, pattern),
        score_threshold);
    // What about in the other direction? (speedup)
    best_loc = text.rfind(pattern, loc + pattern.length());
    if (best_loc != -1) {
      score_threshold = (std::min)(match_bitapScore(0, best_loc, loc, pattern),
          score_threshold);
    }
  }

  // Initialise the bit arrays.
  int matchmask = 1 << (pattern.length() - 1);
  best_loc = -1;

  int bin_min, bin_mid;
  int bin_max = pattern.length() + text.length();
  int *rd;
  int *last_rd = NULL;
  for (int d = 0; d < (int)pattern.length(); d++) {
    // Scan for the best match; each iteration allows for one more error.
    // Run a binary search to determine how far from 'loc' we can stray at
    // this error level.
    bin_min = 0;
    bin_mid = bin_max;
    while (bin_min < bin_mid) {
      if (match_bitapScore(d, loc + bin_mid, loc, pattern)
          <= score_threshold) {
        bin_min = bin_mid;
      } else {
        bin_max = bin_mid;
      }
      bin_mid = (bin_max - bin_min) / 2 + bin_min;
    }
    // Use the result from this iteration as the maximum for the next.
    bin_max = bin_mid;
    int start = (std::max)(1, loc - bin_mid + 1);
    int finish = (std::min)(loc + bin_mid, (int)text.length()) + pattern.length();

    rd = new int[finish + 2];
    rd[finish + 1] = (1 << d) - 1;
    for (int j = finish; j >= start; j--) {
      int charMatch;
      if ((int)text.length() <= j - 1) {
        // Out of range.
        charMatch = 0;
      } else {
        charMatch = s[text[j - 1]];
      }
      if (d == 0) {
        // First pass: exact match.
        rd[j] = ((rd[j + 1] << 1) | 1) & charMatch;
      } else {
        // Subsequent passes: fuzzy match.
        rd[j] = ((rd[j + 1] << 1) | 1) & charMatch
            | (((last_rd[j + 1] | last_rd[j]) << 1) | 1)
            | last_rd[j + 1];
      }
      if ((rd[j] & matchmask) != 0) {
        double score = match_bitapScore(d, j - 1, loc, pattern);
        // This match will almost certainly be better than any existing
        // match.  But check anyway.
        if (score <= score_threshold) {
          // Told you so.
          score_threshold = score;
          best_loc = j - 1;
          if (best_loc > loc) {
            // When passing loc, don't exceed our current distance from loc.
            start = (std::max)(1, 2 * loc - best_loc);
          } else {
            // Already passed loc, downhill from here on in.
            break;
          }
        }
      }
    }
    if (match_bitapScore(d + 1, loc, loc, pattern) > score_threshold) {
      // No hope for a (better) match at greater error levels.
      break;
    }
    delete [] last_rd;
    last_rd = rd;
  }
  delete [] last_rd;
  delete [] rd;
  return best_loc;
}


double diff_match_patch::match_bitapScore(int e, int x, int loc,
                                          const wstring &pattern) const {
  const float accuracy = static_cast<float> (e) / pattern.length();
  const int proximity = (loc - x < 0)? (x - loc) : (loc - x);
  if (Match_Distance == 0) {
    // Dodge divide by zero error.
    return proximity == 0 ? accuracy : 1.0;
  }
  return accuracy + (proximity / static_cast<float> (Match_Distance));
}


void diff_match_patch::match_alphabet(const wstring &pattern, std::map<wchar_t, int>& s) {
  size_t i;
  for (i = 0; i < pattern.length(); i++) {
    wchar_t c = pattern[i];
    s[c] = 0;
  }
  for (i = 0; i < pattern.length(); i++) {
    wchar_t c = pattern[i];
    s[c] = s[c] | (1 << (pattern.length() - i - 1));
  }
}


//  PATCH FUNCTIONS


void diff_match_patch::patch_addContext(Patch &patch, const wstring &text) const {
  if (text.empty()) {
    return;
  }
  wstring pattern = safeMid(text, patch.start2, patch.length1);
  int padding = 0;

  // Look for the first and last matches of pattern in text.  If two different
  // matches are found, increase the pattern length.
  while (text.find(pattern) != text.rfind(pattern)
      && (int)pattern.length() < Match_MaxBits - Patch_Margin - Patch_Margin) {
    padding += Patch_Margin;
    pattern = safeMid(text, (std::max)(0, patch.start2 - padding),
        (std::min)((int)text.length(), patch.start2 + patch.length1 + padding)
        - (std::max)(0, patch.start2 - padding));
  }
  // Add one chunk for good luck.
  padding += Patch_Margin;

  // Add the prefix.
  wstring prefix = safeMid(text, (std::max)(0, patch.start2 - padding),
      patch.start2 - (std::max)(0, patch.start2 - padding));
  if (!prefix.empty()) {
    patch.diffs.push_front(Diff(EQUAL, prefix));
  }
  // Add the suffix.
  wstring suffix = safeMid(text, patch.start2 + patch.length1,
      (std::min)((int)text.length(), patch.start2 + patch.length1 + padding)
      - (patch.start2 + patch.length1));
  if (!suffix.empty()) {
    patch.diffs.push_back(Diff(EQUAL, suffix));
  }

  // Roll back the start points.
  patch.start1 -= prefix.length();
  patch.start2 -= prefix.length();
  // Extend the lengths.
  patch.length1 += prefix.length() + suffix.length();
  patch.length2 += prefix.length() + suffix.length();
}


Patches diff_match_patch::patch_make(const wstring &text1,
                                          const wstring &text2) const {
  // No diffs provided, compute our own.
  Diffs diffs = diff_main(text1, text2, true);
  if (diffs.size() > 2) {
    diff_cleanupSemantic(diffs);
    diff_cleanupEfficiency(diffs);
  }

  return patch_make(text1, diffs);
}


Patches diff_match_patch::patch_make(const Diffs &diffs) const {
  // No origin string provided, compute our own.
  return patch_make(diff_text1(diffs), diffs);
}


Patches diff_match_patch::patch_make(const wstring &text1,
                                          const Diffs &diffs) const {
  Patches patches;
  if (!diffs.empty()) { // Get rid of the null case.
    Patch patch;
    int char_count1 = 0;  // Number of characters into the text1 string.
    int char_count2 = 0;  // Number of characters into the text2 string.
    // Start with text1 (prepatch_text) and apply the diffs until we arrive at
    // text2 (postpatch_text).  We recreate the patches one by one to determine
    // context info.
    wstring prepatch_text = text1;
    wstring postpatch_text = text1;
    for (Diffs::const_iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
      if (patch.diffs.empty() && (*cur_diff).operation != EQUAL) {
        // A new patch starts here.
        patch.start1 = char_count1;
        patch.start2 = char_count2;
      }

      switch ((*cur_diff).operation) {
        case INSERT:
          patch.diffs.push_back(*cur_diff);
          patch.length2 += (*cur_diff).text.length();
          postpatch_text = postpatch_text.substr(0, char_count2)
              + (*cur_diff).text + safeMid(postpatch_text, char_count2);
          break;
        case DELETEA:
          patch.length1 += (*cur_diff).text.length();
          patch.diffs.push_back(*cur_diff);
          postpatch_text = postpatch_text.substr(0, char_count2)
              + safeMid(postpatch_text, char_count2 + (*cur_diff).text.length());
          break;
        case EQUAL:
          if ((int)(*cur_diff).text.length() <= 2 * Patch_Margin
              && !patch.diffs.empty() && !(*cur_diff == diffs.back())) {
            // Small equality inside a patch.
            patch.diffs.push_back(*cur_diff);
            patch.length1 += (*cur_diff).text.length();
            patch.length2 += (*cur_diff).text.length();
          }

          if ((int)(*cur_diff).text.length() >= 2 * Patch_Margin) {
            // Time for a new patch.
            if (!patch.diffs.empty()) {
              patch_addContext(patch, prepatch_text);
              patches.push_back(patch);
              patch = Patch();
              // Unlike Unidiff, our patch lists have a rolling context.
              // http://code.google.com/p/google-diff-match-patch/wiki/Unidiff
              // Update prepatch text & pos to reflect the application of the
              // just completed patch.
              prepatch_text = postpatch_text;
              char_count1 = char_count2;
            }
          }
          break;
      }

      // Update the current character count.
      if ((*cur_diff).operation != INSERT) {
        char_count1 += (*cur_diff).text.length();
      }
      if ((*cur_diff).operation != DELETEA) {
        char_count2 += (*cur_diff).text.length();
      }
    }
    // Pick up the leftover patch if not empty.
    if (!patch.diffs.empty()) {
      patch_addContext(patch, prepatch_text);
      patches.push_back(patch);
    }
  }
  return patches;
}


void diff_match_patch::patch_apply(const Patches &patches, const wstring &sourceText,
                                       std::pair<wstring, std::vector<bool> >& res) const {
  if (patches.empty()) {
    res.first = sourceText;
    res.second.clear();
    return;
  }
  wstring text = sourceText;  // Copy to preserve original.

  // Deep copy the patches so that no changes are made to originals.
//  Patches patchesCopy = patch_deepCopy(patches);
  Patches patchesCopy(patches); // Default copy constructor will do it just fine

  wstring nullPadding = patch_addPadding(patchesCopy);
  text = nullPadding + text + nullPadding;
  patch_splitMax(patchesCopy);

  int x = 0;
  // delta keeps track of the offset between the expected and actual location
  // of the previous patch.  If there are patches expected at positions 10 and
  // 20, but the first patch was found at 12, delta is 2 and the second patch
  // has an effective expected position of 22.
  int delta = 0;
  std::vector<bool>& results = res.second;
  results.resize(patchesCopy.size());
  wstring text1, text2;
  for (Patches::const_iterator cur_patch = patchesCopy.begin(); cur_patch != patchesCopy.end(); ++cur_patch) {
    int expected_loc = (*cur_patch).start2 + delta;
    text1 = diff_text1((*cur_patch).diffs);
    int start_loc;
    int end_loc = -1;
    if ((int)text1.length() > Match_MaxBits) {
      // patch_splitMax will only provide an oversized pattern in the case of
      // a monster delete.
      start_loc = match_main(text, text1.substr(0, Match_MaxBits), expected_loc);
      if (start_loc != -1) {
        end_loc = match_main(text, right(text1, Match_MaxBits),
            expected_loc + text1.length() - Match_MaxBits);
        if (end_loc == -1 || start_loc >= end_loc) {
          // Can't find valid trailing context.  Drop this patch.
          start_loc = -1;
        }
      }
    } else {
      start_loc = match_main(text, text1, expected_loc);
    }
    if (start_loc == -1) {
      // No match found.  :(
      results[x] = false;
      // Subtract the delta for this failed patch from subsequent patches.
      delta -= (*cur_patch).length2 - (*cur_patch).length1;
    } else {
      // Found a match.  :)
      results[x] = true;
      delta = start_loc - expected_loc;
      if (end_loc == -1) {
        text2 = safeMid(text, start_loc, text1.length());
      } else {
        text2 = safeMid(text, start_loc, end_loc + Match_MaxBits - start_loc);
      }
      if (text1 == text2) {
        // Perfect match, just shove the replacement text in.
        text = text.substr(0, start_loc) + diff_text2((*cur_patch).diffs) + safeMid(text, start_loc + text1.length());
      } else {
        // Imperfect match.  Run a diff to get a framework of equivalent
        // indices.
        Diffs diffs = diff_main(text1, text2, false);
        if ((int)text1.length() > Match_MaxBits
            && diff_levenshtein(diffs) / static_cast<float> (text1.length())
            > Patch_DeleteThreshold) {
          // The end points match, but the content is unacceptably bad.
          results[x] = false;
        } else {
          diff_cleanupSemanticLossless(diffs);
          int index1 = 0;
          for (Diffs::const_iterator cur_diff = (*cur_patch).diffs.begin(); cur_diff != (*cur_patch).diffs.end(); ++cur_diff) {
            if ((*cur_diff).operation != EQUAL) {
              int index2 = diff_xIndex(diffs, index1);
              if ((*cur_diff).operation == INSERT) {
                // Insertion
                text = text.substr(0, start_loc + index2) + (*cur_diff).text
                    + safeMid(text, start_loc + index2);
              } else if ((*cur_diff).operation == DELETEA) {
                // Deletion
                text = text.substr(0, start_loc + index2)
                    + safeMid(text, start_loc + diff_xIndex(diffs,
                    index1 + (*cur_diff).text.length()));
              }
            }
            if ((*cur_diff).operation != DELETEA) {
              index1 += (*cur_diff).text.length();
            }
          }
        }
      }
    }
    x++;
  }
  // Strip the padding off.
  res.first = safeMid(text, nullPadding.length(), text.length() - 2 * nullPadding.length());
}


wstring diff_match_patch::patch_addPadding(Patches &patches) const {
  short paddingLength = Patch_Margin;
  wstring nullPadding;
  for (short x = 1; x <= paddingLength; x++) {
    nullPadding += (wchar_t)x;
  }

  // Bump all the patches forward.
  for (Patches::iterator cur_patch = patches.begin(); cur_patch != patches.end(); ++cur_patch) {
    (*cur_patch).start1 += paddingLength;
    (*cur_patch).start2 += paddingLength;
  }

  // Add some padding on start of first diff.
  Patch &firstPatch = patches.front();
  Diffs &firstPatchDiffs = firstPatch.diffs;
  if (firstPatchDiffs.empty() || firstPatchDiffs.front().operation != EQUAL) {
    // Add nullPadding equality.
    firstPatchDiffs.push_front(Diff(EQUAL, nullPadding));
    firstPatch.start1 -= paddingLength;  // Should be 0.
    firstPatch.start2 -= paddingLength;  // Should be 0.
    firstPatch.length1 += paddingLength;
    firstPatch.length2 += paddingLength;
  } else if (paddingLength > (int)firstPatchDiffs.front().text.length()) {
    // Grow first equality.
    Diff &firstDiff = firstPatchDiffs.front();
    int extraLength = paddingLength - firstDiff.text.length();
    firstDiff.text = safeMid(nullPadding, firstDiff.text.length(),
        paddingLength - firstDiff.text.length()) + firstDiff.text;
    firstPatch.start1 -= extraLength;
    firstPatch.start2 -= extraLength;
    firstPatch.length1 += extraLength;
    firstPatch.length2 += extraLength;
  }

  // Add some padding on end of last diff.
  Patch &lastPatch = patches.front();
  Diffs &lastPatchDiffs = lastPatch.diffs;
  if (lastPatchDiffs.empty() || lastPatchDiffs.back().operation != EQUAL) {
    // Add nullPadding equality.
    lastPatchDiffs.push_back(Diff(EQUAL, nullPadding));
    lastPatch.length1 += paddingLength;
    lastPatch.length2 += paddingLength;
  } else if (paddingLength > (int)lastPatchDiffs.back().text.length()) {
    // Grow last equality.
    Diff &lastDiff = lastPatchDiffs.back();
    int extraLength = paddingLength - lastDiff.text.length();
    lastDiff.text += nullPadding.substr(0, extraLength);
    lastPatch.length1 += extraLength;
    lastPatch.length2 += extraLength;
  }

  return nullPadding;
}


void diff_match_patch::patch_splitMax(Patches &patches) const {
  short patch_size = Match_MaxBits;
  wstring precontext, postcontext;
  Patch patch;
  int start1, start2;
  bool empty;
  Operation diff_type;
  wstring diff_text;
  Patch bigpatch;

  for (Patches::iterator cur_patch = patches.begin(); cur_patch != patches.end();) {
    if ((*cur_patch).length1 <= patch_size) { ++cur_patch; continue; }
    bigpatch = *cur_patch;
    // Remove the big old patch.
    cur_patch = patches.erase(cur_patch);
    start1 = bigpatch.start1;
    start2 = bigpatch.start2;
    precontext.clear();
    while (!bigpatch.diffs.empty()) {
      // Create one of several smaller patches.
      patch = Patch();
      empty = true;
      patch.start1 = start1 - precontext.length();
      patch.start2 = start2 - precontext.length();
      if (!precontext.empty()) {
        patch.length1 = patch.length2 = precontext.length();
        patch.diffs.push_back(Diff(EQUAL, precontext));
      }
      while (!bigpatch.diffs.empty()
          && patch.length1 < patch_size - Patch_Margin) {
        diff_type = bigpatch.diffs.front().operation;
        diff_text = bigpatch.diffs.front().text;
        if (diff_type == INSERT) {
          // Insertions are harmless.
          patch.length2 += diff_text.length();
          start2 += diff_text.length();
          patch.diffs.push_back(bigpatch.diffs.front());
          bigpatch.diffs.pop_front();
          empty = false;
        } else if (diff_type == DELETEA && patch.diffs.size() == 1
            && patch.diffs.front().operation == EQUAL
            && (int)diff_text.length() > 2 * patch_size) {
          // This is a large deletion.  Let it pass in one chunk.
          patch.length1 += diff_text.length();
          start1 += diff_text.length();
          empty = false;
          patch.diffs.push_back(Diff(diff_type, diff_text));
          bigpatch.diffs.pop_front();
        } else {
          // Deletion or equality.  Only take as much as we can stomach.
          diff_text = diff_text.substr(0, (std::min)((int)diff_text.length(),
              patch_size - patch.length1 - Patch_Margin));
          patch.length1 += diff_text.length();
          start1 += diff_text.length();
          if (diff_type == EQUAL) {
            patch.length2 += diff_text.length();
            start2 += diff_text.length();
          } else {
            empty = false;
          }
          patch.diffs.push_back(Diff(diff_type, diff_text));
          if (diff_text == bigpatch.diffs.front().text) {
            bigpatch.diffs.pop_front();
          } else {
            bigpatch.diffs.front().text = safeMid(bigpatch.diffs.front().text, diff_text.length());
          }
        }
      }
      // Compute the head context for the next patch.
      precontext = safeMid(diff_text2(patch.diffs), (std::max)(0, (int)precontext.length() - Patch_Margin));
      // Append the end context for this patch.
      postcontext = diff_text1(bigpatch.diffs);
      if ((int)postcontext.length() > Patch_Margin) {
        postcontext = postcontext.substr(0, Patch_Margin);
      }
      if (!postcontext.empty()) {
        patch.length1 += postcontext.length();
        patch.length2 += postcontext.length();
        if (!patch.diffs.empty()
            && patch.diffs.back().operation == EQUAL) {
          patch.diffs.back().text += postcontext;
        } else {
          patch.diffs.push_back(Diff(EQUAL, postcontext));
        }
      }
      if (!empty) {
        patches.insert(cur_patch, patch);
      }
    }
  }
}


wstring diff_match_patch::patch_toText(const Patches &patches) {
  wstring text;
  for (Patches::const_iterator cur_patch = patches.begin(); cur_patch != patches.end(); ++cur_patch) {
    text += (*cur_patch).toString();
  }
  return text;
}


Patches diff_match_patch::patch_fromText(const wstring &textline) const {
  Patches patches;
  if (!textline.empty()) {
    wchar_t sign;
    wstring line;
    wstring::const_pointer text = textline.c_str();
    wstring::size_type text_len, l;
    while (text - textline.c_str() < (int)textline.length()) {
      if ((text_len = next_token(textline, '\n', text)) == 0) { ++text; continue; }

      // A replacement for the regexp "^@@ -(\\d+),?(\\d*) \\+(\\d+),?(\\d*) @@$" exact match
      wstring start1, length1, start2, length2;
      bool success = false;
      wstring::const_pointer t = text;
      l = text_len;
      if ((l -= 9) > 0 && *t == '@' && *++t == '@' && *++t == ' ' && *++t == '-' && std::iswdigit(*++t)) {
        do { start1 += *t; } while (--l > 0 && std::iswdigit(*++t));
        if (l > 0 && *t == ',') ++t, --l;
        while (l > 0 && std::iswdigit(*t)) --l, length1 += *t++;
        if (l > 0 && *t++ == ' ' && *t++ == '+' && std::iswdigit(*t)) {
          do { start2 += *t; } while (--l >= 0 && std::iswdigit(*++t));
          if (l > 0 && *t == ',') ++t, --l;
          while (l > 0 && std::iswdigit(*t)) --l, length2 += *t++;
          if (l == 0 && *t++ == ' ' && *t++ == '@' && *t == '@') success = true;
        }
      }
      if (!success) throw L"Invalid patch string: " + wstring(text, text_len);

      Patch patch;
      patch.start1 = to_int(start1);
      if (length1.empty()) {
        patch.start1--;
        patch.length1 = 1;
      } else if (length1.size() == 1 && length1[0] == '0') {
        patch.length1 = 0;
      } else {
        patch.start1--;
        patch.length1 = to_int(length1);
      }

      patch.start2 = to_int(start2);
      if (length2.empty()) {
        patch.start2--;
        patch.length2 = 1;
      } else if (length2.size() == 1 && length2[0] == '0') {
        patch.length2 = 0;
      } else {
        patch.start2--;
        patch.length2 = to_int(length2);
      }

      for (text += text_len + 1; text - textline.c_str() < (int)textline.length(); text += text_len + 1) {
        if ((text_len = next_token(textline, '\n', text)) == 0) continue;

        sign = *text;
        line.assign(text + 1, text_len - 1);
        percent_decode(line);
        if (sign == '-') {
          // Deletion.
          patch.diffs.push_back(Diff(DELETEA, line));
        } else if (sign == '+') {
          // Insertion.
          patch.diffs.push_back(Diff(INSERT, line));
        } else if (sign == ' ') {
          // Minor equality.
          patch.diffs.push_back(Diff(EQUAL, line));
        } else if (sign == '@') {
          // Start of next patch.
          break;
        } else {
          // WTF?
          throw wstring(L"Invalid patch mode '") + sign + L"' in: " + line;
        }
      }

      patches.push_back(patch);
    }
  }
  return patches;
}
