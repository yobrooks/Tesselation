class GraphicsMath
{
public:
	template <typename T> 
	T dotProduct(vector<T> const &a, vector<T> const &b);

	template<typename T>
	vector<T> crossProduct(vector<T> const &a, vector<T> const &b);

	template<typename T>
	bool lineSegIntersect(vector<T> const &p1, vector<T> const &p2, vector<T> const &p3, vector <T> const &p4);

	template <typename T>
	T areaTriangle(vector<T> const &a, vector<T> const &b, vector<T> const &c);
};




