// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/
//

#include <daw/vector.h>
#include <daw/daw_vector.h>

#include <benchmark/benchmark.h>
#include <cstddef>
#include <vector>

template<typename T>
static void bench_vec_pb( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( T( i ) );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_resize_data_ptr( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( &v );
			v.resize( n );
			T * ptr = v.data( );
			for( std::size_t i = 0; i < n; ++i ) {
				ptr[i] = T( i );	
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}



template<typename T>
static void bench_vec_reserve_pb( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( T( i ) );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_mmap( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, daw::MMapAlloc<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( T( i ) );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_mmap_reserve_pb( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, daw::MMapAlloc<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( T( i ) );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_resize_and_overwrite( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( v.data( ) );
			v.resize_and_overwrite( n, []( T *ptr, std::size_t const N ) {
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
static void bench_vec_mmap_resize_and_overwrite( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, daw::MMapAlloc<T>> v;
			benchmark::DoNotOptimize( v.data( ) );
			v.resize_and_overwrite( n, []( T *ptr, std::size_t const N ) {
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


template<std::size_t N>
static void make_args( benchmark::internal::Benchmark *b ) {
	b->Arg( N );
}

// int
BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )->Apply( make_args<16> );

BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )->Apply( make_args<1024> );

BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )->Apply( make_args<4096> );

BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )->Apply( make_args<16384> );

BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )->Apply( make_args<5242880> );

// long long
BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )->Apply( make_args<16> );

BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )->Apply( make_args<1024> );

BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )->Apply( make_args<4096> );

BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )->Apply( make_args<16384> );

BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )->Apply( make_args<5242880> );

// float
BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )->Apply( make_args<16> );

BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )->Apply( make_args<1024> );

BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )->Apply( make_args<4096> );

BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )->Apply( make_args<16384> );

BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )->Apply( make_args<5242880> );

// double
BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )->Apply( make_args<16> );

BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )->Apply( make_args<1024> );

BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )->Apply( make_args<4096> );

BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )->Apply( make_args<16384> );

BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )->Apply( make_args<5242880> );


