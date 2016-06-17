Code
----
* **Comments**: the reason for some comments varies. Some where placed there to remind myself of how Boost handles stuff, while a few are there to explain what the code by itself cannot. Perhaps I should settle on the latter?

Features
--------
* **Multiple connections**: a possibly most-likely somewhere-future addition. After all, the object *is* called `irc::client` (and a client should be able to handle multiple connections right? Anyway, before doing this, I feel like there should be some way of not having to add both an ssl and a non-ssl socket for each connection, where one of them is never used...)
* **FIFO**: You know what.
