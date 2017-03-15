#include "biginteger.hpp"
#include "matrix.hpp"
#include "elements.hpp"
#include "vector.hpp"
#include "ratlib.hpp"
#include "errors.hpp"
#include <iostream>

int main(int argc, const char * argv[]){
    try {
        FILE *out = fopen("out.txt", "wt");
        FILE *in = fopen("output.txt", "rt");

        const RatNum a = "3/4";
        const RatNum b = 2/a;
        RatNum c, d(b);
        c = a+d;

        c.pravilo();
        printf("c = ");
        c.print();
        printf("\n");

        const RatNum r("44","77");
        printf("r = ");
        r.print();
        printf("\n");

        /*Matrix m1(in);
        FILE *f = fopen("file.txt", "rt");
        Matrix m2(f), m3;
        m2.write(stdout);
        printf("\n");
        m1.write(stdout);

        m3 = m1*m2;
        m3.write(out);*/
      /*FILE *outA = fopen("a.txt", "rt");
      FILE *outB = fopen("b.txt", "rt");
      printf("------\n");
      Vector v1(outA);
      printf("++++++\n");
      v1.write(stdout);
      printf("\n");
      Vector v2(outB);
      printf("++++++\n");
      v2.write(stdout);
      printf("\n");
      RatNum scalar("2","4");
      //scalar = v1*v2;
      scalar.print();

      //C = m1 * B;
      //C.write(stdout);*/

    } catch (znam_ERROR) {
        std::cout << "Ошибка\n";
    }
    catch (delenie_na_0_ERROR) {
        std::cout << "Ошибка";
    }
    catch (memory_allocated_ERROR) {
        std::cout << "Ошибка";
    }
    catch (types_ERROR) {
        std::cout << "Ошибка";
    }
    catch (matrix_razmer_ERROR) {
        std::cout << "Ошибка";
    }
    catch (vector_razmer_ERROR) {
        std::cout << "Ошибка";
    }
    return 0;
}

