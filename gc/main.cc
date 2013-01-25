
extern int program_main(int, char **);

int main(int argc, char **argv) throw() {
    // TODO: setup GC state
    int ret(program_main(argc, argv));
    // TODO: tear down GC state
    return ret;
}
