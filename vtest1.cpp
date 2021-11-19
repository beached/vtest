// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/
//

#include <daw/daw_vector.h>

#include <benchmark/benchmark.h>
#include <cstddef>
#include <vector>

template<typename T>
static void bench_std_vec( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			std::vector<T> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			auto val = T{0};
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( val++ );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_std_vec_no_res( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			std::vector<T> v;
			benchmark::DoNotOptimize( &v );
			auto val = T{0};
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( val++ );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_my_vec( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::Vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			auto val = T{0};
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( val++ );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_my_vec_no_res( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::Vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			auto val = T{0};
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( val++ );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_my_vec_mmap( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::Vector<T, daw::MMapAlloc<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			auto val = T{0};
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( val++ );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_my_vec_mmap_no_res( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::Vector<T, daw::MMapAlloc<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			auto val = T{0};
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( val++ );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_my_vec_rsfo( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::Vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( v.data( ) );
			v.resize_for_overwrite( n, []( T *ptr, std::size_t const N ) {
				auto val = T{0};
				for( std::size_t m = 0; m < N; ++m ) {
					ptr[m] = T( m );
				}
				return N;
			} );
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_my_vec_mmap_rsfo( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::Vector<T, daw::MMapAlloc<T>> v;
			benchmark::DoNotOptimize( v.data( ) );
			v.resize_for_overwrite( n, []( T *ptr, std::size_t const N ) {
				auto val = T{0};
				for( std::size_t m = 0; m < N; ++m ) {
					ptr[m] = T( m );
				}
				return N;
			} );
			benchmark::DoNotOptimize( v.data( ) );
			benchmark::ClobberMemory( );
		}
		( );
	}
}


static void make_args( benchmark::internal::Benchmark *b ) {
	b->Arg( 4096 ); /*
	std::size_t ns[] = { 16, 1024, 4096, 1048576, 5242880 };
	for( auto n : ns ) {
		b->Arg( n );
	}*/
}

BENCHMARK_TEMPLATE( bench_std_vec, int )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_std_vec_no_res, int )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_my_vec, int )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_my_vec_no_res, int )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_my_vec_rsfo, int )->Apply( make_args );

BENCHMARK_TEMPLATE( bench_std_vec, double )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_std_vec_no_res, double )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_my_vec, double )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_my_vec_no_res, double )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_my_vec_rsfo, double )->Apply( make_args );

BENCHMARK_TEMPLATE( bench_std_vec, float )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_std_vec_no_res, float )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_my_vec, float )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_my_vec_no_res, float )->Apply( make_args );
BENCHMARK_TEMPLATE( bench_my_vec_rsfo, float )->Apply( make_args );
