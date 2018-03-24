# NoteNow
For when your thoughts can't wait. Loads up fast. BAM! Right in your face. NOTENOW!

# No bullshit

Literally all it has is file menu shit and clipboard operations. NOTENOW!

# Install dependencies
    sudo apt-get install build-essential libgtk2.0-dev

# Compile
    gcc -o notenow notenow.c `pkg-config --libs --cflags gtk+-2.0`
    
# Run
    ./notenow
