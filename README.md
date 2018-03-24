# Install dependencies
    sudo apt-get install build-essential libgtk-3-dev

# Compile
    gcc -o text text.c `pkg-config --libs --cflags gtk+-2.0`
    
# Run
    ./text
