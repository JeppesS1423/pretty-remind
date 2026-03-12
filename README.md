# pretty-remind
A simple C executable that lists your upcoming reminders with flair.

## Requirements
- [remind](https://dianne.skoll.ca/projects/remind/) installed and accessible in your `PATH`
- A reminders file at `~/.reminders`
- CMake 4.0+
- A C17-compatible compiler (gcc, clang)

## Building
```bash
git clone https://github.com/JeppesS/pretty-remind.git
cd pretty-remind
cmake -B build
cmake --build build
```

The binary will be at `build/pretty-remind`.

## Install

To make it available system-wide:
```bash
sudo cp build/pretty-remind /usr/local/bin/pretty-remind
```

## Usage
```bash
pretty-remind
```

Output is grouped by date, sorted chronologically:
```
Reminders

Wednesday, 25th March
  09:00  Fix that one bug with formating
  12:30  Fika

Thursday, 26th March
  11:20  Train to Stockholm
```

## License

MIT License — Copyright (c) 2026 Jesper Härnlund