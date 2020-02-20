INFICON Spot Library
====================

About
-----
This is a library for the INFICON Spot sensor connected to an Arduino.
It implements the SPI bus communication with the INFICON Spot family,
i.e. the Spot CDS500D [1] and the Spot CDS550D [2]. The interface
specification can be downloaded from the product website.

[1]: https://products.inficon.com/en-us/nav-products/product/detail/spot-cds500d/
[2]: https://products.inficon.com/en-us/nav-products/product/detail/spot-cds550d/

Hardware connection
-------------------

| Arduino           | Spot           |
|-------------------|----------------|
| +5V               | +5V (Pin 1)    |
| GND               | GND (Pin 2)    |
| Pin 13            | Clk (Pin 3)    |
| Pin 10            | SS/ (Pin 4)    |
| Pin 12            | MISO (Pin 5)   |
| Pin 11            | MOSI (Pin 6)   |
| Pin 3             | RDY/ (Pin 7)   |

Pins 8, 9 and 10 of the Spot are left unconnected.

Github
------
The project is available on github at
<https://github.com/INFICON-Spot/inficon-spot-lib>

Bugs
----
Please report bugs to Christian Riesch <christian.riesch@inficon.com>

License
-------
See LICENSE file.
