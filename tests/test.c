void add(int* abc) {
    *abc = 1;
}

int main() {
    add((void*)((int*(&(5)))))
}
