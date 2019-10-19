// Compiled with: g++ -Wall -std=c++14 -pthread
 
//https://codebunk.com/b/838173698/
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstdint>
#include <cstring>
#include <math.h> 
using namespace std;


//DATASET 1

int C = 3000, N = 4;
double poids[4]={0.5,1.5,1.0,1.2};
int calories[4]={500,900,900,200};
float quantite[4]={2,2,3,2};
float quantiteBis[4]; 
double ratio[4];
float res[5]={0,0,0,0,0};
float resFinal[5]={0,0,0,0,0};
int tabConst[5]={-1,-1,-1,-1};


//DATASET 2 
/*
int C = 4000, N = 7;
double poids[7]={2,0.1,0.4,0.9,1.4,1.9,0.1};
int calories[7]={88,295,925,434,655,733,331};
float quantite[7]={2,2,7,7,4,6,4};
float quantiteBis[7]; 
double ratio[7];
float res[8]={0,0,0,0,0,0,0,0};
float resFinal[8]={0,0,0,0,0,0,0,0};
int tabConst[7]={-1,-1,-1,-1,-1,-1,-1};
*/


//DATASET 3
/*
int C = 17000, N = 10;
double poids[10]={1.1,0.9,1.5,1.5,1.3,0.7,2,1,1.8,0.5};
int calories[10]={976,674,793,754,600,231,574,266,362,574};
float quantite[10]={6,5,3,2,3,6,5,3,1,1};
float quantiteBis[10]; 
double ratio[10];
float res[11]={0,0,0,0,0,0,0,0,0,0,0};
float resFinal[11]={0,0,0,0,0,0,0,0,0,0,0};
int tabConst[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
*/




float lighter=999;
int compt = 0;


void afficherTab();
void bAb();
int maximum();
int countCalorie();
void setPoids();
void reset();



//reset les tableaux et met à jours les contraintes
void resetWithConstraint(int constraint[4]){
      for(int i(0); i<N; ++i){
	quantiteBis[i] = quantite[i];
        ratio[i] = calories[i]/poids[i];
        if(constraint[i] >= 0){
                res[i] = constraint[i];
                quantiteBis[i]=0;
                ratio[i]=0;
        }
        else{
        res[i]=0;
        }
    }
    res[N]=0;
}

//récupère l'indice de l'ingrédient avec le meilleur ratio
int maximumCalorie(){
    int max = 0;
    for(int i =0; i < N; i++){
        if( ratio[i] > ratio[max] && quantiteBis[i] > 0){
            max = i;
		
	}
    }
    return max;
}

//compte le nb de calorie total actuel du résultat
int countCalorie(){
    int cal = 0;
    for(int i = 0; i < N+1; i++){
        cal += res[i]*calories[i]; 
    }
    return cal;
}

//set la somme des poids dans le tableau de résultat
void setPoids(){
    float pds = 0;
    for(int i = 0; i < N; i++){
        pds += res[i]*poids[i]; 
    }
    
    res[N] = pds;
}

//teste si il y a encore des éléments dans le tableau des quantités
int testQuantite(){
	
	for(int i = 0; i < N; i++){
		if (quantiteBis[i] > 0)
			return 1;
	}

	return 0;
}


//algorithme principal de résolution, recherche une solution en prenant les meilleurs éléments en fonction de leur ratio
void bAb(){
       compt++;


       cout << "NB iteration: " << compt << endl;
	
       while( countCalorie() < C && testQuantite()>0){
        // on récupére l'ingrédient avec le meilleur ratio

        int bestFood = maximumCalorie();
	
        // tant qu'il y a de cet élément on en prend un
        while(quantiteBis[bestFood] > 0){

            if((countCalorie() + calories[bestFood]) < C ){
		
                quantiteBis[bestFood] -= 1;
                res[bestFood]++;
            }
            else{

                float reste = C - countCalorie();  
                reste = reste/calories[bestFood];
                res[bestFood] += reste;
                quantiteBis[bestFood]--;
            }
            if(quantiteBis[bestFood] == 0)
                ratio[bestFood] = 0;
            
            
            
            //cout << "Nb Calories: " << countCalorie() << endl;
        
        }
    } 
    setPoids();
    cout << "Branch And Bound" << endl;
            for (int i = 0; i < N+1; i++){
                 cout << res[i] << endl;   
            }
    cout << "CURRENT WEIGHT: " << lighter << endl;
    cout << "--------------------------" << endl;
    
}


//détermine le prochain élément non entier sur lequel appliquer le branch and bound
int whosNext(){
    
    for(int i=0; i < N; i++){
        if((res[i] - floor(res[i])) > 0){ 
            return i;
        }
    }
    if(res[N] < lighter){	
    	lighter = res[N];
	for(int i=0; i < N+1; i++){
		resFinal[i] = res[i];	
	}
    }
    return -1;
}

//test s'il est utile de continuer une branche en testant son poids
int continuer(){
    float pds = 0;
    for(int i = 0; i < N; i++){
        pds += res[i]*poids[i]; 
    }
    
    if( pds >= lighter)
	return 0;
    else
	return 1;

}


//le branch and bound
void branchAndBound(int tabConst[]){

    int c1, c2;
    int next = whosNext();
    cout << "next: " << next << " : "  << quantiteBis[next] << endl;
    int stop = continuer();
    if (res[next] >= 0 and next >= 0 and stop > 0){
        int contrainte1[N];
        int contrainte2[N];
        for (int i = 0; i < N; i++){
            contrainte1[i] = tabConst[i];
            contrainte2[i] = tabConst[i];  
        }
         cout << "contrainte: ";
        for(int i(0); i<N; ++i){
               cout << contrainte1[i] << " ";
        }
        cout << endl;
    contrainte1[next]= ceil(res[next]);
    contrainte2[next]= floor(res[next]); 
        
    resetWithConstraint(contrainte1);
    bAb();
    branchAndBound(contrainte1); 
    
         
    resetWithConstraint(contrainte2);
        cout << "contrainte2: ";
        for(int i(0); i<N; ++i){
               cout << contrainte2[i] << " ";
        }
       cout << endl;
    bAb();
    branchAndBound(contrainte2);  
        
    }    
}

//Main function
int main(int argc, char* argv[]){
    for(int i(0); i<N; ++i){
	quantiteBis[i] = quantite[i];
        ratio[i] = calories[i]/poids[i];
	}
	
     bAb();
     
     branchAndBound(tabConst);
    
     afficherTab();

     cout << "Pour tester les autres dataset, decommentez le dataset voulu tout en haut et commentez l'actuel" << endl;	
    
 
 
   
     return EXIT_SUCCESS;
}
 
 
//array screen
void afficherTab(){
        cout << "*************************" << endl;

    cout << "Branch And Bound" << endl;
            for (int i = 0; i < N+1; i++){
                 cout << resFinal[i] << endl;   
            }
    cout << "CURRENT WEIGHT: " << lighter << endl;
    cout << "*************************" << endl;
    
    cout << "Produits     ";
    for(int j(0); j<N; ++j)
        cout << j << "     ";
    cout << endl << "_____________________________________" << endl;
    for(int i(0); i<N   ; ++i){
        switch(i){
            case 0: cout << "Poids    ";
            cout << " | ";
            for(int j(0); j<N; ++j){
                if(poids[j]<=9)
                    cout<<poids[j] << "    ";
                else if(poids[j]<=99)
                    cout<<poids[j] << "   ";
                else
                    cout<<poids[j] << "  ";
            }
            break;
            case 1: cout << "Calories ";
            cout << " | ";
            for(int j(0); j<N; ++j){
                if(calories[j]<=9)
                    cout<<calories[j] << "    ";
                else if(calories[j]<=99)
                    cout<<calories[j] << "   ";
                else
                    cout<<calories[j] << "    ";
            }
            break;
            case 2: cout << "Quantite ";
            cout << " | ";
            for(int j(0); j<N; ++j){
                if(quantiteBis[j]<=9)
                    cout<<quantiteBis[j] << "      ";
                else if(quantiteBis[j]<=99)
                    cout<<quantiteBis[j] << "   ";
                else
                    cout<<quantiteBis[j] << "  ";
            }
            break;
            case 3: cout << "ratio    ";
            cout << " | ";
            for(int j(0); j<N; ++j){
                if(ratio[j]<=9)
                    cout<<ratio[j] << "    ";
                else if(ratio[j]<=99)
                    cout<<ratio[j] << "   ";
                else
                    cout<<ratio[j] << "   ";
            }
            break;
 
        }
        cout<<endl;
    }
}
 

