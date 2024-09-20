import sys




if __name__ == "__main__":
    if len(sys.argv) != 5:
        sys.exit(f"Incorrect arguments\npython {sys.argv[0]} <b1> <b2> <b3> <b4>") 

    byte1 = int(sys.argv[1], 16)
    byte2 = int(sys.argv[2], 16)
    byte3 = int(sys.argv[3], 16)
    byte4 = int(sys.argv[4], 16)

    char = chr(ord('a') + ((byte1 & 0b11111000) >> 3))
    children = ((byte1 & 0b00000111) << 12) | (byte2 << 4) | (byte3 >> 4);
    child_count = ((byte3 & 0b00001111) << 1) | ((byte4 & 0b10000000) >> 7);
    score = (byte4 & 0b01111110) >> 1;
    complete = byte4 & 0b00000001;

    print("char:", char)
    print("children:", children)
    print("child_count:", child_count)
    print("score:", score)
    print("complete:", complete)
