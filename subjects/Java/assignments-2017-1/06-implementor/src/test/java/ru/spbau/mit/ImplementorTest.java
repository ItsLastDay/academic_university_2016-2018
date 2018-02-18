package ru.spbau.mit;

import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;

import javax.tools.*;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.collection.IsArrayWithSize.arrayWithSize;
import static org.hamcrest.core.Is.is;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class ImplementorTest {
    private static final Class<?> IMPLEMENTOR_CLASS;

    static {
        try {
            IMPLEMENTOR_CLASS = Class.forName("ru.spbau.mit.SimpleImplementor");
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }

    @Rule
    public TemporaryFolder testsDirectory = new TemporaryFolder();

    @Rule
    public TemporaryFolder outputDirectory = new TemporaryFolder();

    private String getTestsDirectoryPath() {
        return testsDirectory.getRoot().getPath();
    }

    private String getOutputDirectoryPath() {
        return outputDirectory.getRoot().getPath();
    }

    @Test
    public void shouldBeConstructorWithStringParameter() throws Exception {
        newImplementor();
    }

    private void deleteFolderContent(File folder, boolean isInner) {
        File[] files = folder.listFiles();
        if (files != null) { //some JVMs return null for empty dirs
            for (File f : files) {
                if (f.isDirectory()) {
                    deleteFolderContent(f, true);
                } else {
                    f.delete();
                }
            }
        }
        if (isInner) {
            folder.delete();
        }
    }

    @Before
    public void setUp() throws IOException {
        testsDirectory.create();
        String prefix = "./src/test/java/ru/spbau/mit/testClasses";
        JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
        DiagnosticCollector<JavaFileObject> diagnostics = new DiagnosticCollector<>();
        StandardJavaFileManager fileManager = compiler.getStandardFileManager(diagnostics, null, null);
        List<String> filesToCompile = Arrays.asList("AbstractBaseUtil", "FinalClassUtil", "InterfaceUtil",
                "EmptyInterface");
        List<String> filePaths = filesToCompile.stream()
                .map(path -> Paths.get(prefix, path + ".java")
                        .toAbsolutePath().toString()).collect(Collectors.toList());
        Iterable<? extends JavaFileObject> compilationUnits = fileManager.getJavaFileObjectsFromStrings(
                filePaths);
        List<String> options = new ArrayList<>();
        options.addAll(Arrays.asList("-classpath", System.getProperty("java.class.path") + getTestsDirectoryPath(),
                "-d", getTestsDirectoryPath()));
        JavaCompiler.CompilationTask task = compiler.getTask(null, fileManager, diagnostics, options,
                null, compilationUnits);
        boolean success = task.call();
        if (!success) {
            for (Diagnostic<? extends JavaFileObject> diagnostic : diagnostics.getDiagnostics()) {
                System.err.println(diagnostic.toString());
            }
            throw new RuntimeException();
        }
        fileManager.close();
    }

    @After
    public void cleanUp() {
        deleteFolderContent(new File(getOutputDirectoryPath()), false);
    }

    @Test
    public void implementCloneable() throws Exception {
        checkInterfaceImplementationFromStandardLibrary("java.lang.Cloneable");
    }

    @Test
    public void implementAbstractSet() throws Exception {
        checkAbstractClassImplementationFromStandardLibrary("java.util.AbstractSet");
    }

    @Test
    public void implementFromDefaultPackage() throws Exception {
        checkInterfaceImplementationFromFolder("EmptyInterface");
    }

    @Test(expected = ImplementorException.class)
    public void implementNonexistantStandardClass() throws Exception {
        checkAbstractClassImplementationFromStandardLibrary("java.kotlin.clojure");
    }

    @Test(expected = ImplementorException.class)
    public void implementNonexistantClass() throws Exception {
        checkAbstractClassImplementationFromFolder("blabla");
    }

    @Test(expected = ImplementorException.class)
    public void implementFinalStandardClass() throws Exception {
        checkAbstractClassImplementationFromStandardLibrary("java.lang.String");
    }

    @Test(expected = ImplementorException.class)
    public void implementFinalClass() throws Exception {
        checkAbstractClassImplementationFromFolder("ru.spbau.mit.testClasses.FinalClassUtil");
    }

    @Test
    public void implementSomeInterface() throws Exception {
        checkInterfaceImplementationFromFolder("ru.spbau.mit.testClasses.InterfaceUtil");
    }

    @Test
    public void implementClassWithProtectedMeths() throws Exception {
        checkAbstractClassImplementationFromStandardLibrary("java.net.SocketImpl");
    }

    @Test
    public void implementSomeAbstractClass() throws Exception {
        checkAbstractClassImplementationFromFolder("ru.spbau.mit.testClasses.AbstractBaseUtil");
    }

    private void checkInterfaceImplementationFromFolder(String className) throws Exception {
        Implementor implementor = newImplementor();
        String implClassName = implementor.implementFromDirectory(getTestsDirectoryPath(), className);
        compileAndCheckInterfaceImplementation(className, implClassName);
    }

    private void checkInterfaceImplementationFromStandardLibrary(String className) throws Exception {
        Implementor implementor = newImplementor();
        String implClassName = implementor.implementFromStandardLibrary(className);
        compileAndCheckInterfaceImplementation(className, implClassName);
    }

    private void checkAbstractClassImplementationFromFolder(String className) throws Exception {
        Implementor implementor = newImplementor();
        String implClassName = implementor.implementFromDirectory(getTestsDirectoryPath(), className);
        compileAndCheckAbstractClassImplementation(className, implClassName);
    }

    private void checkAbstractClassImplementationFromStandardLibrary(String className) throws Exception {
        Implementor implementor = newImplementor();
        String implClassName = implementor.implementFromStandardLibrary(className);
        compileAndCheckAbstractClassImplementation(className, implClassName);
    }


    private void compileAndCheckInterfaceImplementation(String className, String implClassName) throws IOException {
        final Class<?> outputClass = compileAndLoadClass(implClassName);
        checkImplementsInterface(className, outputClass);
    }

    private void compileAndCheckAbstractClassImplementation(String className, String implClassName) throws IOException {
        final Class<?> outputClass = compileAndLoadClass(implClassName);
        checkExtendsAbstractClass(className, outputClass);
    }
    
    private void checkExtendsAbstractClass(String className, Class<?> outputClass) {
        assertThat(outputClass.getSuperclass().getCanonicalName(), is(className));
    }

    private Class<?> compileAndLoadClass(String implClassName) throws IOException {
        final String outputAbsolutePath = getAbsolutePath(implClassName);
        tryToCompile(outputAbsolutePath);
        final Class<?> outputClass = loadClassFromTestDirectory(implClassName);
        checkIsNotAbstract(outputClass);
        return outputClass;
    }

    private void checkImplementsInterface(String className, Class<?> aClass) {
        assertThat(aClass.getInterfaces(), arrayWithSize(1));
        assertThat(aClass.getInterfaces()[0].getCanonicalName(), is(className));
    }

    private void checkIsNotAbstract(Class<?> aClass) {
        assertFalse(Modifier.isAbstract(aClass.getModifiers()));
    }

    private void tryToCompile(String outputAbsolutePath) throws IOException {
        assertTrue("Can't compile " + outputAbsolutePath, compileFile(outputAbsolutePath));
    }

    private String getAbsolutePath(String implClassName) {
        final String[] split = implClassName.split("\\.");
        split[split.length - 1] += ".java";
        return Paths.get(getOutputDirectoryPath(), split).toAbsolutePath().toString();
    }

    private boolean compileFile(String absolutePath) throws IOException {
        JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
        DiagnosticCollector<JavaFileObject> diagnostics = new DiagnosticCollector<>();
        StandardJavaFileManager fileManager = compiler.getStandardFileManager(diagnostics, null, null);
        Iterable<? extends JavaFileObject> compilationUnits = fileManager.getJavaFileObjectsFromStrings(
                Arrays.asList(absolutePath));
        List<String> options = new ArrayList<>();
        options.addAll(Arrays.asList("-classpath", System.getProperty("java.class.path") + getTestsDirectoryPath()));
        JavaCompiler.CompilationTask task = compiler.getTask(null, fileManager, diagnostics, options,
                null, compilationUnits);
        boolean success = task.call();
        if (!success) {
            for (Diagnostic<? extends JavaFileObject> diagnostic : diagnostics.getDiagnostics()) {
                System.err.println(diagnostic.toString());
            }
        }
        fileManager.close();
        return success;
    }

    private Class<?> loadClassFromTestDirectory(String className) {
        File outputDirectoryFile = new File(getOutputDirectoryPath());
        File testDirectoryFile = new File(getTestsDirectoryPath());

        try {
            // Convert File to a URL
            URL[] urls = new URL[]{outputDirectoryFile.toURI().toURL(),
                    testDirectoryFile.toURI().toURL()};

            // Create a new class loader with the directory
            ClassLoader cl = new URLClassLoader(urls);

            // Load in the class; MyClass.class should be located in
            // the directory file:/c:/myclasses/com/mycompany
            return cl.loadClass(className);
        } catch (MalformedURLException | ClassNotFoundException ignored) {
            throw new RuntimeException("Class cannot be loaded");
        }
    }

    /*
     * This is constructor without parameters for your Implementor implementation.
     */
    private Implementor newImplementor() throws Exception {
        Constructor<?> constructor = getNoArgConstructor();
        constructor.setAccessible(true);
        return (Implementor) constructor.newInstance(getOutputDirectoryPath());
    }

    private Constructor<?> getNoArgConstructor() throws Exception {
        return IMPLEMENTOR_CLASS.getDeclaredConstructor(String.class);
    }

}
