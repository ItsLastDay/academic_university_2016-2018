class Singleton(type):
    def __init__(cls, *args, **kwargs):
        super().__init__(*args, **kwargs)
        cls.instances_dict = dict()

    def __call__(cls, *args, **kwargs):
        tupled_args = tuple(args)
        tupled_args += tuple(sorted(kwargs.items()))
        instance_hash = hash(tupled_args)

        if instance_hash not in cls.instances_dict:
            instance = super().__call__(*args, **kwargs)
            cls.instances_dict[instance_hash] = instance

        return cls.instances_dict[instance_hash]
