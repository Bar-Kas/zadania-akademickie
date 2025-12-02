import pytest
from main import Student


def test_quantity_increases_on_creation():
    assert Student.quantity == 0
    s1 = Student()
    assert Student.quantity == 1
    s2 = Student()
    s3 = Student()
    assert Student.quantity == 3

def test_give_name_validation_and_representation(capsys):
    s = Student()
    assert not s.name_is_valid()
    s.give_name("Jan", "Kowalski")
    assert s.name == "Jan"
    assert s.last_name == "Kowalski"
    assert s.name_is_valid()
    assert repr(s) == "Jan Kowalski"
    assert str(s) == "Kowalski Jan"
    s.say_hello()
    out = capsys.readouterr().out.strip()
    assert out == "Hello! I'm Jan Kowalski. My index number is 0"

def test_marks_and_average():
    s = Student()
    assert s.get_marks() == []
    s.give_mark(5)
    s.give_mark(3)
    s.give_mark(4)
    assert s.get_marks() == [5, 3, 4]
    assert pytest.approx(s.get_avg_marks()) == (5 + 3 + 4) / 3

