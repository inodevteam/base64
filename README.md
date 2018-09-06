# Base64 Encoder via IRQ
```
ENCODE_ALPHABET[<ID>]{ A..Z a..z 0..9 + / } => OUTPUT
                 |_____________________          |___________________________
                                       |                                     |
            A          B          C    |    X          Y       Z             |
INPUT => 000000|00 0000|0000 00|000000 | 000000|00 NNNN|NNNN NN|NNNNNN      (=)NULL
           A      B       C       D    |   W      X       Y       Z          |
           |______|_______|_______|____|___|______|       |_______|__________|
```
