#pragma once
#include <type_traits>


enum class LoginMean_ID
{
	HOST,
	GEST,
	GEST_BEFORE,
	NON
	
};


LoginMean_ID begin(LoginMean_ID);
LoginMean_ID end(LoginMean_ID);
LoginMean_ID operator*(LoginMean_ID& key);
LoginMean_ID operator++(LoginMean_ID& key);