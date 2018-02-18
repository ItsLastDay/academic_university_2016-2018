
def populate_dict_from_name(dct, cls_name):
    with open('{}.txt'.format(cls_name)) as cls_desc_file:
        for line in cls_desc_file:
            if line.startswith('#'):
                continue

            field_name = line[:line.index(':')] 
            field_value = line[line.index(':') + 2:-1]
            dct[field_name] = field_value


class ClassBase(type):
    @classmethod
    def __prepare__(mcls, name, bases):
        d = dict()
        populate_dict_from_name(d, name)
        return d
