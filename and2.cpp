#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


int numberOfProducts;
int numberOfDividers;
int best = 0;
int base = 0;
vector<int> costs;
int mem[10000][10000];
int mem2[10000][100];
vector<int> mem3[10000];


//The round function is used to round the given price to the nearest multiple of 5. It takes an integer cost as argument.
//It takes cost%5 to find what number the given cost ends in, and then adds or subtracts the correct amount from the given cost
int round(int cost)
{
    int roundedCost;
    int profit = (cost%5);
    switch (profit) {
      case 0:
        roundedCost = cost + 0;
        break;
      case 1:
        roundedCost = cost - 1;
        break;
      case 2:
        roundedCost =  cost - 2;
        break;
      case 3:
        roundedCost =  cost + 2;
        break;
      case 4:
        roundedCost =  cost + 1;
        break;
    }
    return roundedCost;
}

//The subvector function is used to calculate the sum of a part of a given vector. It takes a starting position, an end position, and a vector as arguments
//It loops through the given vector from the startpos to the endpos, and adds the costs of all elements in this vector.
//It then rounds the result to the nearest multiple of 5 using the round function defined above
int subvector(int startpos, int endpos, vector<int> vec)
{
    int result = 0;
    for(int i = startpos; i<endpos; i++)
    {
        result += vec[i];
    }
    return round(result);
}


//this function gets a vector and prints all elements from this vector
//it was only used during testing
void printVector(vector<int> v)
{
    cout << "{";
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }
    cout << "}" << endl;
}


//The groupMaking function is the main part of our algorithm. It takes a vector of products, and int for the price, an int for the number of divs used, and an int for the index as arguments
int groupMaking(vector<int> products, int price, int divsUsed, int index)
{

    int bestVal = best;
    //Here we check if we have reached the end of the products vector.
    //If this is the case, we have found a potential solution, we check if this solution is better than our current best solution, and if that is the case, we save it
    //we then return this found solution
    if (products.size() == 0)
    {
        if (price < best)
        {
            best = price;
        }
        return price;
    }

    //We use memoization to save time in our algorithm.
    //If we have calculated the best price for a given combination of index and divsUsed before, we can just take that result from the array.
    //This saves time, and makes our algorithm much faster.
    if(mem2[index][divsUsed] != 0)
    {
        price += mem2[index][divsUsed];
        if (price < best)
        {
            best = price;
        }
        return price;
    }

    int maxProfit = (numberOfDividers - divsUsed +1) * 2;
    int rest = accumulate(products.begin(), products.end(), 0);

    //If we see that we have already used all of our dividers, we know that we can stop our calculation
    //when the current price+the price of the remaining profits-the maximum profit we can still make is bigger than our current best price, we know that we will never find a better solution in our current branch
    //Because of this, we also know that we can stop this part of our calculation
    if(divsUsed <= numberOfDividers && price+(rest - maxProfit) < best)
    {
        //We now start the main part of this function
        //In this for loop, we take a divider, and put it on every possible position in the list of items.
        //We calculate the price of the objects to the left of the divider, and then use recursion to calculate the cheapest possible price for the elements to the right
        for (int i = 0; i < products.size(); i++)
        {
            //Here we calculate the value of the products to the left of our divider.
            //First we look at the mem array to see if we have already calculated the price for this combination of index and i. If so, we just take the value from the array
            //If it was not calculated already, we calculate it using the subvector function defined earlier
            //We also do something similar to find the correct tempvector
            //the tempvector contains all elements to the right of the divider
            int value;
            if(mem[index][i] != 0)
            {
                value = mem[index][i];
            }
            else{
                value = subvector(0, i + 1, products);
                mem[index][i] = value;
            }
            vector<int> tempVector;
            if(!mem3[index+i+1].empty())
            {
                tempVector = mem3[index+i+1];
            }
            else{
                tempVector = {products.begin() + i + 1, products.end()};
                mem3[index+i+1] = tempVector;
            }


            //we add the calculated value to the price, recursively call the groupMaking function using the updated values, and then subtract the value from the price to get ready for the next iteraton of the forloop
            price += value;
            int temp = groupMaking(tempVector, price, divsUsed+1, index + i + 1);
            price -= value;

            //To find out what the cheapest way is to put the divide the remaining products, we subtract the current price from the price calculated in the recursive function.
            //We do the same for every iteration of the forloop, and compare them to find the cheapest one.
            //This cheapest value is then stored in the mem2 array, which is used for memoization
            int result = temp - price;
            if(result < bestVal && result >= 0 )
            {
                bestVal = result;
            }
        }
        mem2[index][divsUsed] = bestVal;
        return bestVal + price;
    }
    return base;
}


//The getInput function is used to read the inputs.
//First it reads the values for numberOfProducts and numberOfDividers.
//It then loops through the remaining values and stores them in the cost vector
//while reading the costs values, we also add them all together and eventually round them. This gives us the base price we get without using any dividers.

void getInput()
{
    cin >> numberOfProducts;
    cin >> numberOfDividers;
    int totalCost = 0;
    for (int i = 0; i < numberOfProducts; i++)
    {
        int cost;
        cin >> cost;
        costs.push_back(cost);
        totalCost+= cost;
    }
    best = round(totalCost);
}

int main()
{

    getInput();
    //auto start = chrono::steady_clock::now();
    base = best + ((numberOfDividers+1)*2);
    groupMaking(costs, 0, 0, 0);
    cout << best << endl;
    //The following functions were used to time how fast our algorithm was. This was used during testing
    //auto end = chrono::steady_clock::now();
    //auto diff = end - start;
    //cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
    return 0;
}

