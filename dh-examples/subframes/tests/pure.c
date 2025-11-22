typedef struct Pure {
    int a;
    int b;
} Pure;
typedef struct Pure_Cfg {
    int a;
    int b;
} Pure_Cfg;
extern Pure pure_init(int id, Pure_Cfg cfg);
extern void pure_fini(Pure* self);
extern void pure_setA(Pure* self, int a);
extern void pure_setB(Pure* self, int b);
extern int pure_getA(Pure* self);
extern int pure_getB(Pure* self);


int main(void) {
    pure_init(
        value, (Pure_Cfg){
                   .a = 1,
                   .b = 2,
               }
    );
    return 0;
}
