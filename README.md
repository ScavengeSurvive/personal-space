# personal-space.inc

A simple library providing an API for player-to-player interactions such as checking which players are stood near each other and detecting who approached who.

## Installation

Simply add to your `pawn.json`/`pawn.yaml`:

```json
{
    "dependencies": ["ScavengeSurvive/personal-space"]
}
```

Update your `dependencies` directory:

```bash
sampctl package ensure
```

Include in your code and begin using the library:

```pawn
#include <personal-space>
```

## Testing

To test, simply run the package:

```bash
sampctl package run
```

And connect to `localhost:7777` with two players to test.
