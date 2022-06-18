#pragma once

template <class TInheritedClass>
class StaticSingleton
{
protected:
    static TInheritedClass& get_instance()
    {
        static TInheritedClass instance;
        return instance;
    }
};

template <class TInheritedClass>
class PublicSingleton
{
public:
	static TInheritedClass& GetInstance()
	{
		static TInheritedClass instance;
		return instance;
	}
};