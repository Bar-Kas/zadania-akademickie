#!/usr/bin/env python3
import pytest
from main import validate_gmail, validate_date

def test_gmail_valid():
    assert validate_gmail("jan-kowalski1@gmail.com") == True

def test_gmail_invalid_uppercase():
    assert validate_gmail("Jan@gmail.com") == False

def test_gmail_invalid_domain():
    assert validate_gmail("jan@onet.pl") == False

def test_date_valid_dashes():
    assert bool(validate_date("12-05-2023")) == True

def test_date_valid_slashes():
    assert bool(validate_date("01/01/2000")) == True

def test_date_invalid_format():
    assert bool(validate_date("2023.05.12")) == False