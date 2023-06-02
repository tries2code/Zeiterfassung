"Zeiterfassung"(Arbeitstitel) ist ein Hobby-Projekt und hat nicht den Anspruch in einem realen Betrieb Verwendung zu finden.
Das ist mein erstes Projekt mit MySQL und wxWidgets.

Nicht in diesem Repository enthalten ist das benötigte Datenbank-Model, sowie der "Resources"-Ordner.
Beides ist für die Ausführung des Programms notwendig. Wer sich die Dantenbank(MySQL) selbst erstellt, sollte zusätzlich
einen "Resources"-Ordner anlegen und darin die Dateien app_Icon.png und database.xml hinterlegen.
Das Projekt befindet sich noch in der Entwicklung.
Nach Fertigstellung ist geplant das Programm sowie die Datenbank, jeweils in ein eigenes Docker-Image zu installieren.
Verwendete Technologien:
-C++
-wxWidgets
-MySQL


//////////////////////////////////////////////////////////////////////////////////////////////////
////ANLEITUNG und FEATURES////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

Zeit - Erfassen:
    -Nur für angelegte Mitarbeiter möglich
    -Lässt keine Negativen Zeitangaben zu
    -Lässt keine Zeitüberlappung zu
    -Lässt keine Eingaben über der maximalen Arbeitszeit pro Tag(konfigurierbar) zu
    -Eingabe eines abweichenden Enddatums ist möglich

Zeit - Austragen
    -Urlaubs-, Krankheitstage oder ähnliches eintragen
    -Nur für angelegte Mitarbeiter möglich
    -Erst nach Vertrsgastart des Mitarbeiter möglich
    -Lässt keine Negativen Zeitangaben zu
    -Lässt keine Zeitüberlappung zu
    -Prüfft ob innerhalb des angebenenen Zeitraums bereits hinterlegte Arbeitszeiten

Zeit - Übersicht:
    -Zeigt Arbeits- Urlaubs-, Krankheits-s und Sonstigezeiten an.
    -Nur für angelegte Mitarbeiter möglich
    -Nur Jahre in denen der jeweilige Mitarbeiter tätig war stehen zur Auswahl
    -Mitarbeiterauswahl per Dropdown oder Autocomplete
    -Zeigt eine oder alle Monate pro Jahr und Mitarbeiter an
    -Zeigt die Gesamt-Zeit der aktuellen Auswahl in Stunden an


Mitarbeiter - Anlegen:
  -Zur Anlage neuer Mitarbeiter
  -Prüft ob der Benutzername bereits in der Datenbank vorhanden ist, und fordert gegebenenfalls eine Alternative
  -Prüft Altersangabe auf Plausibilität
  -Prüft Altersangabe auf Mindesteintrittsalter(konfigurierbar)
  -Prüft SV-Nummer(Format, Peronenbeogene Daten)

Mitarbeiter - Verwalten:
    -Anpassung Hinterlegter Mitarbeiter-Daten
    -Passwort kann zurückgestzt werden(Initiales Passwort->Konfigurierbar)
    -Mitarbeiter auslagern -> Daten werden archiviert

Einstellungen - Mitarbeiter:
    -Anpassung allgemeiner Einstellungen (Mindest-Eintrittsalter, Mindestlänge für Benutzernamen, etc)

Einstellungen - Tarife:
    -Zeigt Deteils aller Tarife
    -Tarife können gelöscht werden. Gelöschte Tarife können lediglich nicht mehr vergeben werden, laufende Verträge         
     behalten aber ihre Gültigkeit.
    -Tarife anlegen
     