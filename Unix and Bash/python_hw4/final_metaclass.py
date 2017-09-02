import types


def get_funcs_from_classobj(cls, need_only_final=False):
    result_set = set()
    for name in cls.__dict__:
        func = getattr(cls, name)
        if (isinstance(func, types.FunctionType) and
           (not need_only_final or 'function_is_final' in func.__dict__)):
                result_set.add(name)

    return result_set


class WithFinals(type):
    def __init__(cls, name, bases, dct):
        super().__init__(name, bases, dct)
        all_defined_funcs_names = get_funcs_from_classobj(cls)

        for base_cls in cls.__mro__[1:]:
            base_cls_func_names = get_funcs_from_classobj(base_cls)
            final_func_names = get_funcs_from_classobj(base_cls, True)

            shadowed_final_funcs = final_func_names.intersection(all_defined_funcs_names)

            if shadowed_final_funcs:
                err_msg = 'Final function(s) {} of class {} is shadowed in class {}.'
                err_msg = err_msg.format(shadowed_final_funcs, base_cls.__name__, name)
                raise TypeError(err_msg)

            all_defined_funcs_names.update(base_cls_func_names)


def final(f):
    f.function_is_final = True
    return f
