Spirit XXXperimental
======

This is an experimental branch of Spirit X3, which would be hopefully merged into the official X3 finally.


## Cheat Sheet
Here's a list of improvement/features over the official x3-devel branch.

### Core Features
* [Facade-based parser/directive API](https://github.com/jamboree/spirit/wiki/Facade-API)
* Literal extension API `extension::literal<T>`. 
* Phoenix actor support `BOOST_SPIRIT_USE_ACTORS(...)`.
* Rule params support.
* Scalability improvement.

### Directives
* Local variables support `x3::locals<Ts...>(args...)[p]`.
* `lexeme(lex)[p]` support.
* All directives are lazy-args enabled.

### Parsers
* Full char-parser support (char-range, char-set, etc...).
* Numeric literal.
* All parsers are lazy-args enabled.

### Extensions
* `seek(s)[p]` support.
* `advance` parser.
* `regex` parser.
* `x3::reference<P>` adaptor.
* `x3::pos_iterator` and `x3::newl` support.

## Support
If you have any problem, please open an issue on the 'issues' page.
