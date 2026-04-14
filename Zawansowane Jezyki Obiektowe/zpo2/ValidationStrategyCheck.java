package com.validation.strategy;

import java.io.File;
import java.lang.annotation.Annotation;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import com.validation.annotation.ValidationFor;

public class ValidationStrategyCheck {

    private static final Map<Class<? extends Annotation>, ValidationStrategy> strategies = new HashMap<>();

    static {

        try {

            String packageName = "com.validation.strategy";
            String path = packageName.replace('.', '/');

            ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
            URL resource = classLoader.getResource(path);

            if (resource != null) {

                File directory = new File(resource.getFile().replace("%20", " "));

                if (directory.exists() && directory.isDirectory()) {
                    File[] files = directory.listFiles();
                    if (files != null) {
                        for (File file : files) {

                            if (file.getName().endsWith(".class")) {
                                String className = packageName + '.' + file.getName().substring(0, file.getName().length() - 6);
                                Class<?> clazz = Class.forName(className);

                                if (clazz.isAnnotationPresent(ValidationFor.class) && ValidationStrategy.class.isAssignableFrom(clazz)) {

                                    ValidationFor validationFor = clazz.getAnnotation(ValidationFor.class);

                                    ValidationStrategy strategyInstance = (ValidationStrategy) clazz.getDeclaredConstructor().newInstance();

                                    strategies.put(validationFor.value(), strategyInstance);
                                }
                            }
                        }
                    }
                }
            }
        } catch (Exception e) {
            System.err.println("Błąd podczas automatycznego ładowania strategii: " + e.getMessage());
        }
    }

    private ValidationStrategyCheck() {
    }

    public static ValidationStrategy getStrategy(Annotation annotation) {
        return strategies.get(annotation.annotationType()); //
    }
}