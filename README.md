"Zeiterfassung"(Arbeitstitel) ist ein Hobby-Projekt und hat nicht den Anspruch in einem realen Betrieb Verwendung zu finden.
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

Zeiterfassung:
    -Nur für angelegte Mitarbeiter möglich
    -Lässt keine Negativen Zeitangaben zu
    -Lässt keine Zeitüberlappung zu
    -Lässt keine Eingaben über der maximalen Arbeitszeit pro Tag(konfigurierbar) zu
    -Eingabe eines abweichenden Enddatums ist möglich


Übersicht:
    -Nur für angelegte Mitarbeiter möglich
    -Nur Jahre in denen der jeweilige Mitarbeiter tätig war stehen zur Auswahl
    -Zeigt eine oder alle Monate pro Jahr und Mitarbeiter an
    -Zeigt die Gesamt-Zeit der aktuellen Auswahl in Stunden an


Mitarbeiteranlage:
  -Zur Anlage neuer Mitarbeiter
  -Prüft ob der Benutzername bereits in der Datenbank vorhanden ist, und fordert gegebenenfalls eine Alternative
  -Prüft Altersangabe auf Plausibilität
  -Prüft Altersangabe auf Mindesteintrittsalter(konfigurierbar)
