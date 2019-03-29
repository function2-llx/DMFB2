def legal(r, c):
    r = (r - 1) % 7
    c = (c - 1) % 7
    if r < 3 and c < 3:
        return True
    if r == 1 or c == 1:
        return True

    return False

if __name__ == "__main__":
    r, c = 52, 52
    print(r, c)

    for i in range(1, r + 1):
        for j in range(1, c + 1):
            if not legal(i, j):
                print(i, j)
                