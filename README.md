# NoteNow
For when your thoughts can't wait. Loads fast. BAM! It's right there for you. NOTENOW!

# No bullshit

Literally all it has is file menu and clipboard operations. NOTENOW!

# Install dependencies
    sudo apt-get install build-essential libgtk2.0-dev

# Compile
    gcc -o notenow notenow.c `pkg-config --libs --cflags gtk+-2.0`
    
# Run
    ./notenow
