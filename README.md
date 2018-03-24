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

# Install
    $ sudo mv notenow /usr/bin/notenow
    $ sudo mv notenow.desktop /usr/share/applications/notenow.desktop
    $ sudo mv notenow.png /usr/share/icons/hicolor/48x48/apps/notenow.png 
