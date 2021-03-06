// 2000-08-22 Benjamin Kosnik <bkoz@cygnus.com>

// Copyright (C) 2000, 2001, 2002 Free Software Foundation
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// 22.2.1.5 - Template class codecvt [lib.locale.codecvt]

#include <locale>
#include <testsuite_hooks.h>



#ifdef _GLIBCPP_USE___ENC_TRAITS

// Need some char_traits specializations for this to work.
typedef unsigned short			unicode_t;

namespace std
{
  template<>
    struct char_traits<unicode_t>
    {
      typedef unicode_t 	char_type;
      // Unsigned as wint_t is unsigned.
      typedef unsigned long  	int_type;
      typedef streampos 	pos_type;
      typedef streamoff 	off_type;
      typedef mbstate_t 	state_type;
      
      static void 
      assign(char_type& __c1, const char_type& __c2);

      static bool 
      eq(const char_type& __c1, const char_type& __c2);

      static bool 
      lt(const char_type& __c1, const char_type& __c2);

      static int 
      compare(const char_type* __s1, const char_type* __s2, size_t __n)
      { return memcmp(__s1, __s2, __n); }

      static size_t
      length(const char_type* __s);

      static const char_type* 
      find(const char_type* __s, size_t __n, const char_type& __a);

      static char_type* 
      move(char_type* __s1, const char_type* __s2, size_t __n);

      static char_type* 
      copy(char_type* __s1, const char_type* __s2, size_t __n)
      {  return static_cast<char_type*>(memcpy(__s1, __s2, __n)); }

      static char_type* 
      assign(char_type* __s, size_t __n, char_type __a);

      static char_type 
      to_char_type(const int_type& __c);

      static int_type 
      to_int_type(const char_type& __c);

      static bool 
      eq_int_type(const int_type& __c1, const int_type& __c2);

      static int_type 
      eof(); 

      static int_type 
      not_eof(const int_type& __c);
    };
}

/*
> how do I check that these conversions are correct? 
Very easy.  Since all the characters are from ASCII you simply
zero-extend the values.

drepper$ echo 'black pearl jasmine tea' | od -t x1
0000000 62 6c 61 63 6b 20 70 65 61 72 6c 20 6a 61 73 6d
0000020 69 6e 65 20 74 65 61 0a

So the UCS-2 string is

0x0062, 0x006c, 0x0061, ...

You get the idea.  With iconv() you have to take care of the
byte-order, though.  UCS-2 can mean little- or big endian.  Looking at
your result

> $9 = 25856

it shows that the other byte-order is used (25856 == 0x6500).
*/


void
initialize_state(std::__enc_traits& state)
{ state._M_init(); }

// Partial specialization using __enc_traits.
// codecvt<unicode_t, char, __enc_traits>
// UNICODE - UCS2 (big endian)
void test01()
{
  using namespace std;
  typedef codecvt_base::result			result;
  typedef unicode_t				int_type;
  typedef char					ext_type;
  typedef __enc_traits				enc_type;
  typedef codecvt<int_type, ext_type, enc_type>	unicode_codecvt;
  typedef char_traits<int_type>			int_traits;
  typedef char_traits<ext_type>			ext_traits;

  bool 			test = true;
  const ext_type* 	e_lit = "black pearl jasmine tea";
  int 			size = strlen(e_lit);

  char  i_lit_base[50] __attribute__((aligned(__alignof__(int_type)))) = 
  { 
    0x00, 0x62, 0x00, 0x6c, 0x00, 0x61, 0x00, 0x63, 0x00, 0x6b, 0x00, 0x20,
    0x00, 0x70, 0x00, 0x65, 0x00, 0x61, 0x00, 0x72, 0x00, 0x6c, 0x00, 0x20,
    0x00, 0x6a, 0x00, 0x61, 0x00, 0x73, 0x00, 0x6d, 0x00, 0x69, 0x00, 0x6e,
    0x00, 0x65, 0x00, 0x20, 0x00, 0x74, 0x00, 0x65, 0x00, 0x61, 0x00, 0xa0
  };
  const int_type* 	i_lit = reinterpret_cast<int_type*>(i_lit_base);

  const ext_type*       efrom_next;
  const int_type*       ifrom_next;
  ext_type* 		e_arr = new ext_type[size + 1];
  ext_type*		eto_next;
  int_type* 		i_arr = new int_type[size + 1];
  int_type*		ito_next;

  // construct a locale object with the specialized facet.
  locale 		loc(locale::classic(), new unicode_codecvt);
  // sanity check the constructed locale has the specialized facet.
  VERIFY( has_facet<unicode_codecvt>(loc) );
  const unicode_codecvt&	cvt = use_facet<unicode_codecvt>(loc); 

  // in
  unicode_codecvt::state_type state01("UCS-2BE", "ISO-8859-15", 0xfeff, 0);
  initialize_state(state01);
  // internal encoding is bigger because of bom
  result r1 = cvt.in(state01, e_lit, e_lit + size, efrom_next, 
		     i_arr, i_arr + size + 1, ito_next);
  VERIFY( r1 == codecvt_base::ok );
  VERIFY( !int_traits::compare(i_arr, i_lit, size) ); 
  VERIFY( efrom_next == e_lit + size );
  VERIFY( ito_next == i_arr + size );

  // out
  unicode_codecvt::state_type state02("UCS-2BE", "ISO-8859-15", 0xfeff, 0);
  initialize_state(state02);  
  result r2 = cvt.out(state02, i_lit, i_lit + size, ifrom_next, 
		       e_arr, e_arr + size, eto_next);
  VERIFY( r2 == codecvt_base::ok );
  VERIFY( !ext_traits::compare(e_arr, e_lit, size) ); 
  VERIFY( ifrom_next == i_lit + size );
  VERIFY( eto_next == e_arr + size );

  // unshift
  ext_traits::copy(e_arr, e_lit, size);
  unicode_codecvt::state_type state03("UCS-2BE", "ISO-8859-15", 0xfeff, 0);
  initialize_state(state03);
  result r3 = cvt.unshift(state03, e_arr, e_arr + size, eto_next);
  VERIFY( r3 == codecvt_base::noconv );
  VERIFY( !ext_traits::compare(e_arr, e_lit, size) ); 
  VERIFY( eto_next == e_arr );

  int i = cvt.encoding();
  VERIFY( i == 2 ); // Target-dependent.

  VERIFY( !cvt.always_noconv() );

  unicode_codecvt::state_type state04("UCS-2BE", "ISO-8859-15", 0xfeff, 0);
  initialize_state(state04);
  int j = cvt.length(state03, e_lit, e_lit + size, 5);
  VERIFY( j == 5 );

  int k = cvt.max_length();
  VERIFY( k == 1 );

  delete [] e_arr;
  delete [] i_arr;
}

// Partial specialization using __enc_traits.
// codecvt<unicode_t, char, __enc_traits>
// UNICODE - UCS2 (little endian)
void test02()
{
  using namespace std;
  typedef codecvt_base::result			result;
  typedef unsigned short			unicode_t;
  typedef unicode_t				int_type;
  typedef char					ext_type;
  typedef __enc_traits				enc_type;
  typedef codecvt<int_type, ext_type, enc_type>	unicode_codecvt;
  typedef char_traits<int_type>			int_traits;
  typedef char_traits<ext_type>			ext_traits;

  bool 			test = true;
  const ext_type* 	e_lit = "black pearl jasmine tea";
  int 			size = strlen(e_lit);

  char  i_lit_base[50] __attribute__((aligned(__alignof__(int_type)))) = 
  { 
    0x62, 0x00, 0x6c, 0x00, 0x61, 0x00, 0x63, 0x00, 0x6b, 0x00, 0x20, 0x00,
    0x70, 0x00, 0x65, 0x00, 0x61, 0x00, 0x72, 0x00, 0x6c, 0x00, 0x20, 0x00,
    0x6a, 0x00, 0x61, 0x00, 0x73, 0x00, 0x6d, 0x00, 0x69, 0x00, 0x6e, 0x00,
    0x65, 0x00, 0x20, 0x00, 0x74, 0x00, 0x65, 0x00, 0x61, 0x00, 0xa0, 0x00
  };
  const int_type* 	i_lit = reinterpret_cast<int_type*>(i_lit_base);

  const ext_type*       efrom_next;
  const int_type*       ifrom_next;
  ext_type* 		e_arr = new ext_type[size + 1];
  ext_type*		eto_next;
  int_type* 		i_arr = new int_type[size + 1];
  int_type*		ito_next;

  // construct a locale object with the specialized facet.
  locale 		loc(locale::classic(), new unicode_codecvt);
  // sanity check the constructed locale has the specialized facet.
  VERIFY( has_facet<unicode_codecvt>(loc) );
  const unicode_codecvt&	cvt = use_facet<unicode_codecvt>(loc); 

  // in
  unicode_codecvt::state_type state01("UCS-2LE", "ISO-8859-15", 0xfeff, 0);
  initialize_state(state01);
  // internal encoding is bigger because of bom
  result r1 = cvt.in(state01, e_lit, e_lit + size, efrom_next, 
		     i_arr, i_arr + size + 1, ito_next);
  VERIFY( r1 == codecvt_base::ok );
  VERIFY( !int_traits::compare(i_arr, i_lit, size) ); 
  VERIFY( efrom_next == e_lit + size );
  VERIFY( ito_next == i_arr + size );

  // out
  unicode_codecvt::state_type state02("UCS-2LE", "ISO-8859-15", 0xfeff, 0);
  initialize_state(state02);  
  result r2 = cvt.out(state02, i_lit, i_lit + size, ifrom_next, 
		       e_arr, e_arr + size, eto_next);
  VERIFY( r2 == codecvt_base::ok );
  VERIFY( !ext_traits::compare(e_arr, e_lit, size) ); 
  VERIFY( ifrom_next == i_lit + size );
  VERIFY( eto_next == e_arr + size );

  // unshift
  ext_traits::copy(e_arr, e_lit, size);
  unicode_codecvt::state_type state03("UCS-2LE", "ISO-8859-15", 0xfeff, 0);
  initialize_state(state03);
  result r3 = cvt.unshift(state03, e_arr, e_arr + size, eto_next);
  VERIFY( r3 == codecvt_base::noconv );
  VERIFY( !ext_traits::compare(e_arr, e_lit, size) ); 
  VERIFY( eto_next == e_arr );

  int i = cvt.encoding();
  VERIFY( i == 2 ); // Target-dependent.

  VERIFY( !cvt.always_noconv() );

  unicode_codecvt::state_type state04("UCS-2LE", "ISO-8859-15", 0xfeff, 0);
  initialize_state(state04);
  int j = cvt.length(state03, e_lit, e_lit + size, 5);
  VERIFY( j == 5 );

  int k = cvt.max_length();
  VERIFY( k == 1 );

  delete [] e_arr;
  delete [] i_arr;
}

#endif // _GLIBCPP_USE___ENC_TRAITS

int main ()
{
#if _GLIBCPP_USE___ENC_TRAITS
  test01();
  test02();
#endif 

  return 0;
}
