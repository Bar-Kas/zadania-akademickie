package com.validation; 

import com.validation.exception.ValidationException; 
import com.validation.validator.Validator; 

public class main { 
    public static void main(String[] args) { 
        try { 
            Pracownik pracownik = new Pracownik();
            pracownik.setEmail("Grzegorz.Brzeczkyszczykiewicz#firma.pl");
            pracownik.setImie("AAAAAAAAAA");
            pracownik.setNazwisko("BBBBBBBBBBB");
            pracownik.setNrIndeksu("a");

            Validator.validate(pracownik); 
        } catch (ValidationException e) { 
            System.out.println(e.getMessage()); 
        } 
    } 
} 