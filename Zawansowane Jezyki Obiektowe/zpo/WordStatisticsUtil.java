package com.lab.statistics;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collectors;

public class WordStatisticsUtil {

    public static Map<String, Long> getLinkedCountedWords(Path path, int wordsLimit) {
        try (BufferedReader reader = Files.newBufferedReader(path)) { 
            return reader.lines()
                    
                    .flatMap(line -> Arrays.stream(line.split("\\s+")))
                    .map(word -> word.replaceAll("[^a-zA-Z0-9ąęćłńóśźżĄĘĆŁŃÓŚŹŻ]", ""))
                    .filter(word -> word.length() > 2)
                    .map(String::toLowerCase)
                    
                    .collect(Collectors.groupingBy(Function.identity(), Collectors.counting()))
                    .entrySet().stream()
                    
                    .sorted(Map.Entry.<String, Long>comparingByValue(Comparator.reverseOrder())) 
                    
                    .limit(wordsLimit)
                    
                    .collect(Collectors.toMap(
                            Map.Entry::getKey,
                            Map.Entry::getValue,
                            (k, v) -> { throw new IllegalStateException(String.format("Błąd! Duplikat klucza %s.", k)); },
                            LinkedHashMap::new
                    ));
        } catch (IOException e) {
            throw new RuntimeException("Wystąpił błąd podczas przetwarzania pliku: " + path.getFileName(), e);
        }
    }
}