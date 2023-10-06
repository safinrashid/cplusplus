// ECE 312 PA0
// Safin Rashid
// srr3288
// Slip days used: 0
// Fall 2022


double computeSquare(double x) {
	return x*x;
}

int gcd(int y, int z) {

    int lowr=y,gcdret;
    if(y < 0){y = -y;}
    if(z < 0){z = -z;}
    if(y == z){return y;}
    else if(z < y){lowr = z;}
    for(int i = 1; i <= lowr; i++){
        if(y%i == 0 && z%i == 0){
            gcdret = i;
        }
    }
	return gcdret;
}

//Given function to be used in findSumPrimes
//Returns 1 if num is prime and 0 if num is not prime
int isPrime(int num) {

    if (num <= 1) return 0;
    if (num % 2 == 0 && num > 2) return 0;
    for(int i = 3; i< num / 2; i+= 2){
        if( num % i == 0 ){
            return 0;
        }
    }
    return 1;
}

int findSumPrimes(int x) {

	int sum = 0;
    for(int i = 1; i <= x; i++){
        if(isPrime(i) == 1){
            sum += i;
        }
    }

	return sum;
}

