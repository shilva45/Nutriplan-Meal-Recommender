#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>

bool alEgg = false;
bool alDairy = false;
bool alGluten = false;
bool alSoy = false;
bool alSeafood = false;
bool alShellfish = false;
bool alPoultry = false;

int caloriesLeft;
int proteinLeft;
int fatLeft;
int carboLeft;

bool sortCal(const std::tuple<std::string, int, int, int, int>& a, const std::tuple<std::string, int, int, int, int>& b){
	return std::get<1>(a) < std::get<1>(b);
}

bool sortProt(const std::tuple<std::string, int, int, int, int>& a, const std::tuple<std::string, int, int, int, int>& b){
	return std::get<2>(a) < std::get<2>(b);
}

bool sortFat(const std::tuple<std::string, int, int, int, int>& a, const std::tuple<std::string, int, int, int, int>& b){
	return std::get<3>(a) < std::get<3>(b);
}

bool sortCarbo(const std::tuple<std::string, int, int, int, int>& a, const std::tuple<std::string, int, int, int, int>& b){
	return std::get<4>(a) < std::get<4>(b);
}

std::string comparePerc(double cal, double protein, double fat, double carbo){
	// find the minimum of all bars. that is the priority nutrient that the user needs the most
	double mn = std::min({cal, protein, fat, carbo});
	
	if(mn == cal) return "cal";
	if(mn == protein) return "protein";
	if(mn == fat) return "fat";
	if(mn == carbo) return "carbo";
	
	return "error";
}

void leftover(int caloriesGoal, int proteinGoal, int fatGoal, int carboGoal){
	using foodTuple = std::tuple<std::string, int, int, int, int>;

    std::vector<foodTuple> foodItems;

	// menu for leftovers. exclude foods that contain user's allergies accordingly
    if(!alEgg) foodItems.emplace_back("Egg(s)", 70, 6, 5, 1);
    if(!alDairy) foodItems.emplace_back("Cup(s) of Milk", 150, 8, 8, 12);
    if(!alGluten) foodItems.emplace_back("Slice(s) of Wheat Bread", 75, 3, 2, 15);
    if(!alDairy) foodItems.emplace_back("Slice(s) of Cheddar Cheese", 110, 7, 9, 1);
    foodItems.emplace_back("Portion(s) of White Rice", 130, 2, 0, 29);
    foodItems.emplace_back("Piece(s) of Sausage", 300, 15, 25, 5);
    if(!alSoy && !alDairy) foodItems.emplace_back("Cup(s) of Soy Milk", 80, 7, 4, 4);
    if(!alDairy) foodItems.emplace_back("Cup(s) of Low-Fat Yogurt", 150, 13, 8, 11);
    if(!alGluten) foodItems.emplace_back("Bowl(s) of Oatmeal", 150, 6, 3, 27);
    foodItems.emplace_back("Apple(s)", 95, 1, 1, 25);
    
    int foodItemsSize = foodItems.size();
    
    // shuffle the initial menu
    auto randSeed = std::default_random_engine{static_cast<unsigned>(time(0))};
    std::shuffle(foodItems.begin(), foodItems.end(), randSeed);
    
    int currCal = 0;
    int currProtein = 0;
    int currFat = 0;
    int currCarbo = 0;
    
    // each nutrient has a bar that starts at 100%. each food item that is added, the bar is reduced accordingly
    double calPerc = (double)(caloriesGoal - currCal) / caloriesGoal * 100;
    double protPerc = (double)(proteinGoal - currProtein) / proteinGoal * 100;
    double fatPerc = (double)(fatGoal - currFat) / fatGoal * 100;
    double carboPerc = (double)(carboGoal - currCarbo) / carboGoal * 100;
    
    bool canConsume = false;
    std::vector<int> foodMenu;
    
    // loop and add food items until none of the food items can be added without surpassing the limit for any nutrient
    do {
    	canConsume = false;
    	
    	for(int i = 0; i < foodItemsSize; i++){
    		const foodTuple& currTuple = foodItems[i];
    		
    		// if food is valid
    		if(currCal + std::get<1>(currTuple) < caloriesGoal && currProtein + std::get<2>(currTuple) < proteinGoal && 
			   currFat + std::get<3>(currTuple) < fatGoal && currCarbo + std::get<4>(currTuple) < carboGoal){
				canConsume = true;
				foodMenu.push_back(i);
				currCal += std::get<1>(currTuple);
				caloriesLeft -= std::get<1>(currTuple);
				currProtein += std::get<2>(currTuple);
				proteinLeft -= std::get<2>(currTuple);
				currFat += std::get<3>(currTuple);
				fatLeft -= std::get<3>(currTuple);
				currCarbo += std::get<4>(currTuple);
				carboLeft -= std::get<4>(currTuple);
			}
		}
		
		// recalculate the bar for each nutrient. all nutrients are likely the have different values for their bar
	    calPerc = (double)(caloriesGoal - currCal) / caloriesGoal * 100;
	    protPerc = (double)(proteinGoal - currProtein) / proteinGoal * 100;
	    fatPerc = (double)(fatGoal - currFat) / fatGoal * 100;
	    carboPerc = (double)(carboGoal - currCarbo) / carboGoal * 100;
	    
	    std::string priority = comparePerc(calPerc, protPerc, fatPerc, carboPerc);
	    
	    // sort the menu by which nutrient the user needs most
	    if(priority == "cal") std::sort(foodItems.begin(), foodItems.end(), sortCal);
	    else if(priority == "protein") std::sort(foodItems.begin(), foodItems.end(), sortProt);
	    else if(priority == "fat") std::sort(foodItems.begin(), foodItems.end(), sortFat);
	    else if(priority == "carbo") std::sort(foodItems.begin(), foodItems.end(), sortCarbo);
	    else {
	    	puts("Error in sorting occured");
	    	return;
		}
		
	} while(canConsume);
	
	std::unordered_map<int, int> foodList;
	
	for(auto i : foodMenu){
		foodList[i]++;
	}
	
	puts("");
	printf("We also recommend you eat the following foods throughout the day:\n");
	
	for(auto it = foodList.begin(); it != foodList.end(); it++){
	    const foodTuple& currTuple = foodItems[it->first];
	    std::cout << "- " << it->second << " " << std::get<0>(currTuple) << "\n";
	}
}

void lunch(){
	using foodTuple = std::tuple<std::string, int, int, int, int>;

    std::vector<foodTuple> foodItems;
    
	// menu for lunch. exclude foods that contain user's allergies accordingly
    foodItems.emplace_back("Plate of Fried Rice", 300, 6, 15, 31);
    foodItems.emplace_back("Bowl of Kwetiau", 315, 9, 11, 43);
    if(!alEgg && !alPoultry) foodItems.emplace_back("Chicken Salad w/ Egg", 384, 28, 28, 3);
    foodItems.emplace_back("Portion of Chicken Curry", 219, 20, 12, 8);
    foodItems.emplace_back("Bowl of Stir-fried Water Spinach", 211, 6, 19, 9);
    if(!alPoultry) foodItems.emplace_back("Bowl of Chicken Porridge", 372, 28, 12, 36);
    if(!alShellfish) foodItems.emplace_back("Bowl of Shrimp Tempura", 187, 23, 3, 13);
    foodItems.emplace_back("Bowl of Nasi Pecel", 553, 16, 23, 76);
    foodItems.emplace_back("Vegetable Soup w/ White Rice", 207, 5, 2, 41);
    if(!alEgg && !alSoy) foodItems.emplace_back("Egg Tofu Soup", 503, 18, 39, 32);
    
    int foodItemsSize = foodItems.size();
    
    bool canConsume = false;
    std::vector<int> foodMenu;
    
    // pick two random food items
    int randomNum = rand() % foodItemsSize;
    
    const foodTuple& currTuple = foodItems[randomNum];
	foodMenu.push_back(randomNum);
	caloriesLeft -= std::get<1>(currTuple);
	proteinLeft -= std::get<2>(currTuple);
	fatLeft -= std::get<3>(currTuple);
	carboLeft -= std::get<4>(currTuple);
	
	randomNum = rand() % foodItemsSize;
	
	const foodTuple& currTuple2 = foodItems[randomNum];
	foodMenu.push_back(randomNum);
	caloriesLeft -= std::get<1>(currTuple2);
	proteinLeft -= std::get<2>(currTuple2);
	fatLeft -= std::get<3>(currTuple2);
	carboLeft -= std::get<4>(currTuple2);
	
	std::unordered_map<int, int> foodList;
	
	for(auto i : foodMenu){
		foodList[i]++;
	}
	
	puts("");
	printf("We recommend you eat the following meals for lunch:\n");
	
	for(auto it = foodList.begin(); it != foodList.end(); it++){
	    const foodTuple& currTuple = foodItems[it->first];
	    std::cout << "- " << it->second << " " << std::get<0>(currTuple) << "\n";
	}
}

void dinner(){
	using foodTuple = std::tuple<std::string, int, int, int, int>;

    std::vector<foodTuple> foodItems;

	// menu for dinner. exclude foods that contain user's allergies accordingly
    foodItems.emplace_back("Plate of Spaghetti Carbonara", 422, 23, 24, 29);
    foodItems.emplace_back("Plate of Nasi Lemak", 448, 35, 12, 50);
    foodItems.emplace_back("Piece of Steak w/ Mashed Potato", 462, 32, 19, 39);
    foodItems.emplace_back("Bowl of Red Braised Pork Belly", 295, 17, 20, 8);
    if(!alSeafood) foodItems.emplace_back("Bowl of Steamed Codfish w/ White Rice", 342, 36, 4, 38);
    if(!alPoultry) foodItems.emplace_back("Bowl of Hainanese Chicken Rice", 403, 23, 15, 42);
    foodItems.emplace_back("Serving of Claypot Rice", 439, 22, 17, 49);
    foodItems.emplace_back("Serving of Yakiniku Beef w/ White Rice", 413, 19, 18, 40);
    foodItems.emplace_back("Serving of Wanton Noodles", 555, 15, 14, 96);
    if(!alPoultry) foodItems.emplace_back("Roasted Duck w/ White Rice", 421, 19, 24, 29);
    
    int foodItemsSize = foodItems.size();
    
    bool canConsume = false;
    std::vector<int> foodMenu;
    
    // pick a random food item
    int randomNum = rand() % foodItemsSize;
    
    const foodTuple& currTuple = foodItems[randomNum];
	foodMenu.push_back(randomNum);
	caloriesLeft -= std::get<1>(currTuple);
	proteinLeft -= std::get<2>(currTuple);
	fatLeft -= std::get<3>(currTuple);
	carboLeft -= std::get<4>(currTuple);
	
	std::unordered_map<int, int> foodList;
	
	for(auto i : foodMenu){
		foodList[i]++;
	}
	
	lunch();
	
	puts("");
	printf("We recommend you eat the following meal for dinner:\n");
	
	for(auto it = foodList.begin(); it != foodList.end(); it++){
	    const foodTuple& currTuple = foodItems[it->first];
	    std::cout << "- " << it->second << " " << std::get<0>(currTuple) << "\n";
	}
	
	leftover(caloriesLeft, proteinLeft, fatLeft, carboLeft);
}

int main(){
	srand(time(NULL));
	
	// user inputs their data
	std::string userGender;
	do {
		std::cout << "Enter your gender: ";
		std::cin >> userGender;
	} while (!(userGender == "Male" || userGender == "Female"));
	
	int userAge;
	std::cout << "Enter your age: ";
	std::cin >> userAge;
	
	int userWeight;
	std::cout << "Enter your weight in kg: ";
	std::cin >> userWeight;
	
	int userHeight;
	std::cout << "Enter your height in cm: ";
	std::cin >> userHeight;
	
	double bmr;
	if(userGender == "Male") bmr = 66 + (13.7 * userWeight) + (5 * userHeight) - (6.8 * userAge);
	else bmr = 655 + (9.6 * userWeight) + (1.8 * userHeight) - (4.7 * userAge);
	
	puts("");
	int activityScore = 0;
	std::cout << "How often do you exercise?\n";
	std::cout << "1. Little to none\n";
	std::cout << "2. Light exercise a few days a week\n";
	std::cout << "3. Moderate exercise most days in a week\n";
	std::cout << "4. Hard exercise daily\n";
	std::cout << "5. Extreme exercise daily\n";
	
	double tdee;
	do {
		std::cout << ">> ";
		std::cin >> activityScore;
		switch(activityScore){
			case 1:
				tdee = bmr * 1.2;
				break;
			case 2:
				tdee = bmr * 1.375;
				break;
			case 3:
				tdee = bmr * 1.55;
				break;
			case 4:
				tdee = bmr * 1.725;
				break;
			case 5:
				tdee = bmr * 1.9;
				break;
			default:
				break;
		}
	} while(activityScore < 1 || activityScore > 5);
	
	// sets global variables of calories, protein, fat, and carbo that user needs
	caloriesLeft = (int)tdee;
	proteinLeft = tdee * 0.25 / 4;
	fatLeft = tdee * 0.3 / 9;
	carboLeft = tdee * 0.45;
	
	puts("");
	int allergyMenu = 0;
	do {
		puts("What are you allegic to?");
		if(!alEgg) puts("1. Eggs");
		if(!alDairy) puts("2. Dairy Products");
		if(!alGluten) puts("3. Gluten");
		if(!alSoy) puts("4. Soy");
		if(!alSeafood) puts("5. Seafood");
		if(!alShellfish) puts("6. Shellfish");
		if(!alPoultry) puts("7. Poultry");
		puts("8. That is all");
		printf(">> ");
		std::cin >> allergyMenu;
		
		switch(allergyMenu){
			case 1:
				alEgg = true;
				break;
			case 2:
				alDairy = true;
				break;
			case 3:
				alGluten = true;
				break;
			case 4:
				alSoy = true;
				break;
			case 5:
				alSeafood = true;
				break;
			case 6:
				alShellfish = true;
				break;
			case 7:
				alPoultry = true;
				break;
			case 8:
				break;
			default:
				break;
			
		}
		puts("");
	} while(allergyMenu != 8);
	
	getchar();
	puts("OK! Generating menu for you...");
	getchar();
	
	puts("Based on the data we have, others with the same sicknesses have warmly received the following recommendations:");
	
	// calls dinner function, which calls lunch and leftover function
	dinner();
	
	// if user does not meet 75% of nutrient requirement, add additional recommendations
	if(caloriesLeft / tdee > 0.25){
		puts("");
		puts("We also recommend eating more foods that are high in calories to fulfill your daily needs, such as:");
		puts("- Nuts and Seeds");
		puts("- Avocado");
		if(!alDairy) puts("- Cheese");
		puts("- Bacon");
	}
	
	if(proteinLeft / (tdee * 0.25 / 4) > 0.25){
		puts("");
		puts("We also recommend eating more foods that are rich in protein to fulfill your daily needs, such as:");
		if(!alSeafood) puts("- Fish");
		if(!alEgg) puts("- Egg White");
		if(!alPoultry) puts("- Chicken Breast");
		if(!alSoy) puts("- Tofu");
		if(!alShellfish) puts("- Shrimp");
		puts("- Beef");
	}
	
	if(fatLeft / (tdee * 0.3 / 9) > 0.25){
		puts("");
		puts("We also recommend eating more foods that are rich in fat to fulfill your daily needs, such as:");
		if(!alDairy) puts("- Cream Cheese");
		if(!alEgg) puts("- Egg Yolk");
		puts("- Sausage");
		if(!alSeafood) puts("- Fatty Fish");
	}
	
	if(carboLeft / (tdee * 0.45) > 0.25){
		puts("");
		puts("We also recommend eating more foods that are rich in carbohydrates to fulfill your daily needs, such as:");
		puts("- Cucumbers");
		puts("- Lettuce");
		puts("- Celery");
		puts("- Zucchini");
		puts("- Spinach");
		puts("- Broccoli");
	}
	
	puts("");
	
	// feedback system
	std::string feedbackChoice;
	puts("Would you be willing to leave us a review and some feedback? (Yes/No)");
	do {
		printf(">> ");
		std::cin >> feedbackChoice;
		
		if(feedbackChoice == "No"){
			break;
		} else if(feedbackChoice == "Yes"){
			int ratingChoice = 0;
			std::string feedbackInput;
			
			puts("How well would you rate our product and recommend it to others? (1-10)");
			
			do {
				printf(">> ");
				std::cin >> ratingChoice;
				
				
				if(ratingChoice <= 5){
					puts("We are sorry to hear that you are disatisfied with Nutriplan. Which part of our product should we improve on?");
					printf(">> ");
					std::cin >> feedbackInput;
					puts("");
					printf("We will review your feedback and seek to improve Nutriplan. ");
					break;
				} else if(ratingChoice > 5){
					puts("We are glad to hear that you enjoyed using Nutriplan. Which part of our product should we improve on?");
					printf(">> ");
					std::cin >> feedbackInput;
					puts("");
					printf("We will review your feedback and seek to improve Nutriplan. ");
					break;
				}
				
			} while(ratingChoice < 1 || ratingChoice > 10);
		}
		
	} while (!(feedbackChoice == "Yes" || feedbackChoice == "No"));
	
	puts("Thank you for using our product. We hope to see you again soon!");
	getchar();
	puts("");
	puts("Exit...");
	getchar();
	
    return 0;
}

