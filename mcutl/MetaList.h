
#pragma once

#include "MetaUtils.h"

namespace meta
{
	/////////////////////////////////////////////////////////////////////////
	// Type list
	/////////////////////////////////////////////////////////////////////////

	template<typename... Types> struct List;
	template<> struct List<> { static const uint8_t size = 0; };
	template<typename Head,typename... Tail> struct List<Head,Tail...>
	{
		typedef Head          head;
		typedef List<Tail...> tail;
		static const int size = tail::size + 1;
	};

	/////////////////////////////////////////////////////////////////////////
	// Prepend type to list
	/////////////////////////////////////////////////////////////////////////

	template<typename Type,class List> struct PushFront;
	template<typename Type,typename... Tail> struct PushFront<Type,List<Tail...>>
	{
		typedef List<Type,Tail...> result;
	};

	/////////////////////////////////////////////////////////////////////////
	// Take first
	/////////////////////////////////////////////////////////////////////////

	template<class List,uint8_t Num> struct TakeFirst;
	template<> struct TakeFirst<List<>,0> { typedef List<> result; };
	template<typename Head,typename... Tail> struct TakeFirst<List<Head,Tail...>,0> { typedef List<> result; };
	template<typename Head,typename... Tail,uint8_t Num> struct TakeFirst<List<Head,Tail...>,Num>
	{
		typedef typename PushFront<
			Head,
			typename TakeFirst<List<Tail...>,Num-1>::result
		>::result result;
	};

	/////////////////////////////////////////////////////////////////////////
	// Skip first
	/////////////////////////////////////////////////////////////////////////

	template<class List,uint8_t Num> struct SkipFirst;
	template<> struct SkipFirst<List<>,0> { typedef List<> result; };
	template<typename Head,typename... Tail> struct SkipFirst<List<Head,Tail...>,0> { typedef List<Head,Tail...> result; };
	template<typename Head,typename... Tail,uint8_t Num> struct SkipFirst<List<Head,Tail...>,Num>
	{
		typedef typename SkipFirst<List<Tail...>,Num-1>::result result;
	};

	/////////////////////////////////////////////////////////////////////////
	// Transform type list using functor Func<Type>::result
	/////////////////////////////////////////////////////////////////////////

	template<template<typename> class Func,class List> struct Transform;
	template<template<typename> class Func> struct Transform<Func,List<>> { typedef List<> result; };
	template<template<typename> class Func,typename Head,typename... Tail> struct Transform<Func,List<Head,Tail...>>
	{
		typedef typename Func<Head>::result head;
		typedef typename Transform<Func,List<Tail...>>::result tail;
		typedef typename PushFront<head,tail>::result result;
	};

	/////////////////////////////////////////////////////////////////////////
	// Transform type list using functor Func<Type,Index>::result
	/////////////////////////////////////////////////////////////////////////

	template<template<typename,uint8_t> class Func,class List,uint8_t Start=0> struct TransformI;
	template<template<typename,uint8_t> class Func,uint8_t I> struct TransformI<Func,List<>,I> { typedef List<> result; };
	template<template<typename,uint8_t> class Func,typename Head,typename... Tail,uint8_t I> struct TransformI<Func,List<Head,Tail...>,I>
	{
		typedef typename Func<Head,I>::result head;
		typedef typename TransformI<Func,List<Tail...>,I+1>::result tail;
		typedef typename PushFront<head,tail>::result result;
	};

	/////////////////////////////////////////////////////////////////////////
	// Filter type list using predicate Func<Type>::value
	/////////////////////////////////////////////////////////////////////////

	template<template<typename> class Func,class List> struct Filter;
	template<template<typename> class Func> struct Filter<Func,List<>> { typedef List<> result; };
	template<template<typename> class Func,typename Head,typename... Tail> struct Filter<Func,List<Head,Tail...>>
	{
		typedef typename Filter<Func,List<Tail...>>::result tail;
		typedef typename StaticIf<
			Func<Head>::value,
			typename PushFront<Head,tail>::result,
			tail
		>::result result;
	};

	/////////////////////////////////////////////////////////////////////////
	// Remove all entries of given type
	/////////////////////////////////////////////////////////////////////////

	template<typename Type,class List> struct RemoveAll
	{
		template<typename T> struct Func { static const bool value = !IsEqual<T,Type>::value; };
		typedef typename Filter<Func,List>::result result;
	};

	/////////////////////////////////////////////////////////////////////////
	// Remove all non-unique elements from list
	/////////////////////////////////////////////////////////////////////////

	template<class List> struct Unique;
	template<> struct Unique<List<>> { typedef List<> result; };
	template<typename Head,typename... Tail> struct Unique<List<Head,Tail...>>
	{
		typedef typename Unique<
			typename RemoveAll<Head,List<Tail...>>::result
		>::result tail;
		typedef typename PushFront<Head,tail>::result result;
	};

	/////////////////////////////////////////////////////////////////////////
	// Indexed list
	/////////////////////////////////////////////////////////////////////////

	template<typename Type,uint8_t Index> struct IndexedType
	{
		typedef Type type;
		static constexpr uint8_t index = Index;
	};
	template<typename T,uint8_t I> struct MakeIndexedType { typedef IndexedType<T,I> result; };
	template<typename... Types> struct MakeIndexedList { typedef typename TransformI<MakeIndexedType,List<Types...>>::result result; };
}
