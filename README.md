Spirit XXXperimental
======

                   _     _   _      __   __ _____
         ___ _ __ (_)_ _(_)_| |_    \ \ / / __  /
        / __| '_ \| | '/| |_   _|    \ V /   / /
        \__ \ |_) | | | | | | |_     / . \   \ \
        |___/ .__/|_|_| |_| \___|   / / \ \ --  \
            |_|                     --   -- -----
              P a r s e r   F r a m e w o r k

This is an experimental branch of Spirit X3.

## Cheat Sheet
Here's a list of improvement/features over the official x3-devel branch.

### Core Features
* [Facade-based parser/directive API](https://github.com/jamboree/spirit/wiki/Facade-API)
* Literal extension API `extension::literal<T>`. See [example](include/boost/spirit/home/x3/numeric/int.hpp#L67).
* Phoenix actor support `BOOST_SPIRIT_USE_ACTORS(...)`.
* Rule params support.
* [Scalability improvement](https://github.com/jamboree/spirit/wiki/Scalability).

### Directives
* Local variables support `x3::locals<Ts...>(args...)[p]`. See [example](test/x3/rule3.cpp#L120).
* `x3::lexeme(lex)[p]` support. See [example](test/x3/lexeme.cpp#L37).
* All directives are lazy-args enabled.

### Parsers
* Full char-parser support (char-range, char-set, etc...). See [example](test/x3/char1.cpp#L106).
* Numeric literal.
* All parsers are lazy-args enabled.

### Extensions
* `x3::seek(stepper)[p]` support. See [example](test/x3/seek.cpp#L60).
* `x3::sink(attr)[p]` support. See [example](test/x3/sink.cpp).
* `x3::advance` parser. See [example](test/x3/advance.cpp).
* `x3::regex` parser. See [example](test/x3/regex.cpp).
* `x3::reference<P>` adaptor.
* `x3::pos_iterator` and `x3::newl` support. See [example](test/x3/pos_iterator.cpp).

## Support
If you have any problem, please open an issue on the [issues](https://github.com/jamboree/spirit/issues) page.
