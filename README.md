Zadanie 2: Readers Writers and Producers
========================================

W aplikacji działa procesów `N` pisarzy (writers), `M` czytelników (readers) oraz `2N` producentów (producers).
Każdy z pisarzy ma przypisanych dwóch producentów z którymi jest połączony z pomocą pipe'ów.
Każdy producent jest przypisany do dokładnie jednego pisarza.
Połączenie jest realizowane za pomocą pipe'a tak że producent ma otwarty ten pipe do pisania a pisarz do czytania.
Nikt więcej w systemie nie ma dostępu do takiego pipe'a.
Po stronie pisarza pipe'y do producentów są otwarte (do czytania) na deskryptorach 3 oraz 4.

Producenci w trakcie swojego działania generują liczby typu `int` i wypisują je binarnie do pipe'ów łączących je z pisarzami.
Każdy z producentów generuje liczby w seriach o (pseudo-)losowej długości oddzielanych przerwami o (pseudo-)losowej długości.
Każdy z producentów kiedyś zakończy swoje działanie.

Każdy z czytelników ma swoje standardowe wyjście przekierowane do przypisanego mu pliku.

Zadanie polega implementacji procesów czytelników i pisarzy w taki sposób aby liczby generowane przez producentów trafiły do plików przypisanych czytelnikom.
Liczby powinny być zapisane tekstowo w zapisie dziesiętnym w kolejnych liniach pliku.
Każda liczba powinna w sumie pojawić się w plikach czytelników tyle razy ile razy została wygenerowana przez producentów.

Można przyjąć że wszystkie opisane powyżej procesy będą miały wspólnego rodzica, który nie zakończy swojego działania dopóki działa jeden z ww procesów.
Każdy z pisarzy otrzyma w pierwszym argumencie sumaryczną liczbę pisarzy, którzy zostaną uruchomieni w trakcie działania aplikacji.

Dodatkowo:

- Należy zadbać o to aby żaden z czytelników nie był wyróżniony. Powinno to skutkować tym że pliki z liczbami zapisanymi przez różnych czytelników będą mniej więcej tego samego rozmiaru.
- Żadna forma aktywnego czekania nie jest dopuszczalna w rozwiązaniu.
- Nie wolno dopuścić do sytuacji gdy pisarz jest zablokowany na jednym z pipe'ów od producentów podczas gdy w drugim dostępne są dane.
- Aplikacja powinna działać poprawnie również w sytuacji gdy niektórzy pisarze rozpoczynają pracę z opóźnieniem. W szczególności może się zdarzyć że część pisarzy zakończy swoją pracę zanim druga część zacznie swoją. Można przyjąć że wszyscy czytelnicy rozpoczynają pracę mniej więcej równocześnie.
- Po zakończeniu działania aplikacji należy usunąć wszystkie tworzone przez siebie pliki/fifo.

Rozwiązanie powinno polegać na implementacji programów dla czytelników i pisarzy w plikach w plikach `reader.c` oraz `writer.c`.

Rozwiązanie będzie testowane w systemie Linux.

Pliki wejściowe dla programu testującego rozwiązanie `rw` w kolejnych liniach zawierają:
- liczba czytelników,
- liczba pisarzy,
- ciąg dodatkowych opóźnień przed startami kolejnych pisarzy (np. `1 2 3` oznacza że pierwszy wystartuje później o 1ms drugi o 3ms a trzeci o 6ms).
