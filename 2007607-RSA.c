#include<stdio.h>
#include<mpir.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

void randomnumber(mpz_t random)
{
	gmp_randstate_t x;
	gmp_randinit_default(x);
	gmp_randseed_ui(x, time(NULL));
	mpz_t p;
	mpz_init(p);
	mpz_rrandomb(p, x, 512); // chooses p in the range [(2^511),(2^512)-1].
	mpz_nextprime(p, p); //takes the next prime of random mpz_t p, i loved that func.
	mpz_add_ui(random, p, 0);	
}

void eulerphi(mpz_t phi, mpz_t p, mpz_t q)
{
	mpz_t a;
	mpz_init(a);
	mpz_t b;
	mpz_init(b);
	mpz_sub_ui(a, p, 1);
	mpz_sub_ui(b, q, 1);
	mpz_mul(phi, a, b);

}

int main()
{

	printf(" Choose one of the options\n");
	DO:                                           // for a wrong choice.
	printf(" Enter 1 to generate parameters\n");
	printf(" Enter 2 to encryption\n");
	printf(" Enter 3 to decryption\n");

	int x;
	scanf_s("%d", &x);

	if (x == 1)
	{
		mpz_t p;
		mpz_init(p);
		mpz_t q;
		mpz_init(q);
		mpz_t n;
		mpz_init(n);
		mpz_t phi;
		mpz_init(phi);
		mpz_t d;
		mpz_init(d);

		randomnumber(p);
		mpz_nextprime(q, p); 

		eulerphi(phi, p, q);
		mpz_mul(n, p, q);


		gmp_randstate_t y;
		gmp_randinit_default(y);
		gmp_randseed_ui(y, time(NULL));
		mpz_t e;
		mpz_init(e);
		mpz_t gcd;
		mpz_init(gcd);
		do {

			mpz_urandomm(e, y, phi);//chooses random e in the range [0,phi-1]
			mpz_add_ui(e, e, 2);
			mpz_gcd(gcd, e, phi);

		} while (mpz_cmp_ui(gcd, 1) != 0);
		
		
		mpz_invert(d, e, phi); //takes the inverse directly.

		/*mpz_t o;    //checks that (d,e)=1.
		mpz_init(o);
		mpz_mul(o, d, e);
		mpz_mod(o, o, phi);
		gmp_printf("%Zd", o);*/


		FILE *gp;
		errno_t err;
		err = fopen_s(&gp,"parameters.txt", "w+");
		gmp_fprintf(gp, "%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd", &p, &q, &phi, &n, &e, &d);
		fclose(gp);

		mpz_clear(p);
		mpz_clear(q);
		mpz_clear(n);
		mpz_clear(phi);
		mpz_clear(d);
		mpz_clear(e);
		mpz_clear(gcd);
	}
	else if (x == 2)
	{
		mpz_t p;
		mpz_init(p);
		mpz_t q;
		mpz_init(q);
		mpz_t n;
		mpz_init(n);
		mpz_t phi;
		mpz_init(phi);
		mpz_t d;
		mpz_init(d);
		mpz_t e;
		mpz_init(e);

		FILE *fp;
		errno_t err;
		err = fopen_s(&fp,"parameters.txt", "r");
		gmp_fscanf(fp, "%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd", &p, &q, &phi, &n, &e, &d);
		fclose(fp);


		FILE *pp;
		errno_t er5;
		char plaintext[CHAR_MAX];        // if my plaintext has x-characters then,
		char* file = "plain.txt";        // this takes x+1-chars with adding a end-line char at the end.
		er5 = fopen_s(&pp,file, "r");    // So it will take at most 127 input chars and +1 char for end line = 128 chars from plain.txt.

		while (fgets(plaintext, CHAR_MAX, pp) != NULL)

		fscanf_s(pp,"%s", plaintext,CHAR_MAX);
		fclose(pp);

		int k = strlen(plaintext);


		mpz_t m;
		mpz_init_set_ui(m, 0);
		mpz_t a;
		mpz_init(a);
		mpz_t radix;
		mpz_init_set_ui(radix, 256); // since we are working with ASCII-256.
		mpz_t base;
		mpz_init(base);

		

		for (int i = 0; i < k; i++)
		{
			mpz_pow_ui(base, radix, i);
			mpz_mul_ui(a, base, (int)plaintext[i]);
			mpz_add(m, m, a);
		}
		

		mpz_t enc;
		mpz_init(enc);
		mpz_powm(enc, m, e, n); //m^e=enc (modn)

		FILE *cp;
		errno_t er1;
		er1 = fopen_s(&cp,"cipher.txt", "w");
		gmp_fprintf(cp, "%Zd", &enc);
		fclose(cp);

		mpz_clear(p);
		mpz_clear(q);
		mpz_clear(n);
		mpz_clear(phi);
		mpz_clear(d);
		mpz_clear(e);
		mpz_clear(m);
		mpz_clear(a);
		mpz_clear(base);
		mpz_clear(radix);
		mpz_clear(enc);

	}
	else if (x == 3)
	{

		mpz_t p;
		mpz_init(p);
		mpz_t q;
		mpz_init(q);
		mpz_t n;
		mpz_init(n);
		mpz_t phi;
		mpz_init(phi);
		mpz_t d;
		mpz_init(d);
		mpz_t e;
		mpz_init(e);
		mpz_t enc;
		mpz_init(enc);

		FILE *againfp;
		errno_t err;
		err = fopen_s(&againfp,"parameters.txt", "r");
		gmp_fscanf(againfp, "%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd", &p, &q, &phi, &n, &e, &d);
		fclose(againfp);

		FILE *againcp;
		errno_t er2;
		er2 = fopen_s(&againcp,"cipher.txt", "r");
		gmp_fscanf(againcp, "%Zd", &enc);
		fclose(againcp);

		mpz_t m;
		mpz_init(m);
		mpz_powm(m, enc, d, n); // m=enc^d (modn)


		int t = 0;
		char message[CHAR_MAX]; //could say 128 instead of CHAR_MAX.
		mpz_t mod;             
		mpz_init(mod);
		mpz_t quot;
		mpz_init(quot);
		

		for (int i = 0; i < CHAR_MAX; i++)
		{
			mpz_mod_ui(mod, m, 256); 
			mpz_div_ui(m, m, 256); 
			
			message[t] = (char)mpz_get_ui(mod);
			//message[t] = mpz_get_ui(mod);
			t = t + 1;

			if (mpz_cmp_ui(m, 0) == 0)
				break;
		}
		
		FILE *lp;
		errno_t er3;
		er3 = fopen_s(&lp,"message.txt", "w");
		for (int i = 0; i < t; i++)
		{
			fprintf(lp, "%c", message[i]);
		}
		fclose(lp);

		mpz_clear(p);
		mpz_clear(q);
		mpz_clear(n);
		mpz_clear(phi);
		mpz_clear(d);
		mpz_clear(e);
		mpz_clear(m);
		mpz_clear(quot);
		mpz_clear(mod);
		mpz_clear(enc);

	}

	else
	{
		printf("You entered a wrong integer, try again\n");
		goto DO;
	}
	return 0;
}



