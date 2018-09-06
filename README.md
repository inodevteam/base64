# Base64 Encoder via IRQ
```
ENCODE_ALPHABET[<ID>]{ A..Z a..z 0..9 + / } => OUTPUT
                 |_____________________
                                       |
            A0        B0        C0     |
INPUT => 000000|00 0000|0000 00|000000 |
           A1     B1      C1      D1   |
           |______|_______|_______|____|
```
