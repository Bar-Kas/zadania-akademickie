package com.validation.strategy;

import java.lang.reflect.Field;
import java.util.Optional;
import java.util.regex.Pattern;

import com.validation.annotation.Email;
import com.validation.annotation.ValidationFor;

@ValidationFor(Email.class)
public class EmailStrategy implements ValidationStrategy {

    private static final String EMAIL_REGEX = "^[A-Za-z0-9+_.-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,6}$";
    private static final Pattern PATTERN = Pattern.compile(EMAIL_REGEX);

    @Override
    public Optional<String> validate(Field field, Object value) {


        if (field.isAnnotationPresent(Email.class) && value != null) {
            String emailValue = (String) value;

            if (!PATTERN.matcher(emailValue).matches()) {
                Email annotation = field.getAnnotation(Email.class);
                String errorInfo = String.format("Pole %s: %s", field.getName(), annotation.message());
                return Optional.of(errorInfo);
            }
        }

        return Optional.empty();
    }
}