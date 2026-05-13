#include <aurora/main.h>

extern "C" int port_main(int argc, char* argv[]);

int main(int argc, char *argv[])
{
    return port_main(argc, argv);
}
