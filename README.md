# Yin

A _Yin_ is an base `2 ^ 11` representation, where each digit is expressed as a consonant
sandwiched between two vowels. This forms a 3-tuple of characters, which are
enumerated lexicographically.  Since there are 5 vowels (`aeiou`), and 21
consonants (everything else), there are `21 * 5 * 21 = 2205` possible
combinations.  Since a _Yin_ digit is restricted to `2 ^ 11 = 2048` values,
the top `2205 - 2048 = 157` combinations are discarded, making `yin` the
heighest value _Yin_ digit.

## Usage

```
$ make
$ echo 1234567890 | ./yin # this utility reads from STDIN and writes to STDOUT
```

## Caution

There are bad 3 letter words. Not all numbers are PC in _Yin_ representation.
