#! /usr/bin/env python3

import cProfile


def prof(filename):
    def decorator(func):
        profiler = cProfile.Profile()

        def wrapped_func(*args, **kwargs):
            profiler.enable()

            result = func(*args, **kwargs)

            profiler.disable()
            # This will dump cumulative stats up to this moment.
            profiler.dump_stats(filename)

            return result

        return wrapped_func

    return decorator
