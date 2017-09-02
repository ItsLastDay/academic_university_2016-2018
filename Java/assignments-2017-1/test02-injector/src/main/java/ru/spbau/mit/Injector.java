package ru.spbau.mit;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.*;
import java.util.stream.Collectors;


public final class Injector {
    private Injector() {
    }

    /**
     * Create and initialize object of `rootClassName` class using classes from
     * `implementationClassNames` for concrete dependencies.
     */
    public static Object initialize(String rootClassName, List<String> implementationClassNames) throws Exception {
        List<Class<?>> loadedClasses = implementationClassNames.stream().map(className -> {
            try {
                return Class.forName(className);
            } catch (ClassNotFoundException e) {
                throw new RuntimeException("Class should exist");
            }
        }).collect(Collectors.toList());
        Class<?> curClass = Class.forName(rootClassName);
        loadedClasses.add(curClass);

        return dfs(curClass, loadedClasses, new HashSet<>(), new HashMap<>());
    }

    private static Object dfs(Class<?> rootClass,
                              List<Class<?>> implementationClasses,
                              Set<Class<?>> currentlyNeedToVisit,
                              Map<Class<?>, Object> alreadyHaveImpl)
            throws InjectionCycleException,
            ClassNotFoundException, IllegalAccessException, InvocationTargetException,
            InstantiationException, ImplementationNotFoundException,
            AmbiguousImplementationException {
        if (alreadyHaveImpl.containsKey(rootClass)) {
            return alreadyHaveImpl.get(rootClass);
        }

        if (currentlyNeedToVisit.contains(rootClass)) {
            throw new InjectionCycleException();
        }

        currentlyNeedToVisit.add(rootClass);

        Constructor<?>[] declaredConstructors = rootClass.getDeclaredConstructors();
        if (declaredConstructors.length != 1) {
            throw new RuntimeException("There should be exactly one constructor");
        }

        Constructor<?> constructor = declaredConstructors[0];
        Class<?>[] neededParameterTypes = constructor.getParameterTypes();

        Object answer = null;

        if (neededParameterTypes.length == 0) {
            answer = constructor.newInstance();
        } else {
            List<Object> paramObjs = new ArrayList<>();
            for (Class<?> neededType : neededParameterTypes) {
                Class<?> implementationClass = getImplementation(neededType, implementationClasses);
                paramObjs.add(dfs(implementationClass, implementationClasses,
                        currentlyNeedToVisit, alreadyHaveImpl));
            }

            answer = constructor.newInstance(paramObjs.toArray(new Object[paramObjs.size()]));
        }

        currentlyNeedToVisit.remove(rootClass);
        alreadyHaveImpl.put(rootClass, answer);
        return answer;
    }


    private static Class<?> getImplementation(Class<?> neededType, List<Class<?>> implementationClasses)
            throws ImplementationNotFoundException, AmbiguousImplementationException {
        Class<?> impl = null;
        for (Class<?> implClass : implementationClasses) {
            boolean isMatch = false;
            if (implClass.equals(neededType)) {
                isMatch = true;
            }

            if (neededType.isInterface()) {
                isMatch = checkClassImplements(implClass, neededType);
            } else {
                isMatch = checkClassExtends(implClass, neededType);
            }

            if (isMatch) {
                if (impl != null) {
                    throw new AmbiguousImplementationException();
                }
                impl = implClass;
            }
        }

        if (impl == null) {
            throw new ImplementationNotFoundException();
        }

        return impl;
    }

    private static boolean checkClassExtends(Class<?> implClass, Class<?> neededType) {
        while (implClass.getSuperclass() != null) {
            if (implClass.equals(neededType)) {
                return true;
            }
            implClass = implClass.getSuperclass();
        }
        return false;
    }

    private static boolean checkClassImplements(Class<?> implClass, Class<?> neededType) {
        if (implClass == null) {
            return false;
        }

        for (Class<?> interf : implClass.getInterfaces()) {
            if (interf.equals(neededType) || checkClassImplements(interf, neededType)) {
                return true;
            }
        }

        return checkClassImplements(implClass.getSuperclass(), neededType);
    }
}
