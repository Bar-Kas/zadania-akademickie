import builtins
import pytest
from Bartosz_Kasprzycki_124405_3 import task_6, task_7


def test_task6_basic_input(monkeypatch, capsys):
    inputs = iter(["1", "2", "2", "3", "0"])
    monkeypatch.setattr(builtins, "input", lambda _: next(inputs))

    task_6()
    output = capsys.readouterr().out.strip()
    assert "{1, 2, 3}" in output or "{1, 3, 2}" in output


def test_task6_empty_input(monkeypatch, capsys):
    inputs = iter(["0"])
    monkeypatch.setattr(builtins, "input", lambda _: next(inputs))

    task_6()
    output = capsys.readouterr().out.strip()
    assert "set()" in output


def test_task7_tuple_behavior(capsys):
    task_7()
    output = capsys.readouterr().out.strip().splitlines()

    assert output[0] == "4"

    expected_items = {"apple", "banana", "cherry", "orange"}
    found_items = set(output[1:])
    assert found_items == expected_items
