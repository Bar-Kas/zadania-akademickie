package com.validation.strategy;

import java.lang.reflect.Field;
import java.util.Optional;
import java.util.regex.Pattern;

import com.validation.annotation.NrIndeksu;
import com.validation.annotation.ValidationFor;

@ValidationFor(NrIndeksu.class)
public class NrIndeksuStrategy implements ValidationStrategy {

    private static final String NR_INDEKSU_REGEX = "^\\d{8}$";
    private static final Pattern PATTERN = Pattern.compile(NR_INDEKSU_REGEX);

    @Override
    public Optional<String> validate(Field field, Object value) {
        if (field.isAnnotationPresent(NrIndeksu.class) && value != null) {
            String stringValue = String.valueOf(value).trim();

            if (!PATTERN.matcher(stringValue).matches()) {
                NrIndeksu annotation = field.getAnnotation(NrIndeksu.class);
                String errorInfo = String.format("Pole %s: %s", field.getName(), annotation.message());
                return Optional.of(errorInfo);
            }
        }
        return Optional.empty();
    }
}