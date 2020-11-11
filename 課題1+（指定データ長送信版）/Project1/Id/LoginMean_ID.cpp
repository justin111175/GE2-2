#include "LoginMean_ID.h"


LoginMean_ID begin(LoginMean_ID)
{
	return LoginMean_ID::HOST;
}

LoginMean_ID end(LoginMean_ID)
{
	return LoginMean_ID::NON;
}

LoginMean_ID operator*(LoginMean_ID& key)
{
	return key;
}

LoginMean_ID operator++(LoginMean_ID& key)
{
	return key = static_cast<LoginMean_ID>(std::underlying_type<LoginMean_ID>::type(key) + 1);
}