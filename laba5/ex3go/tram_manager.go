package main

import (
	"errors"
	"fmt"
	"sort"
)

type TramManager struct {
	tramRoutes map[string][]string    // Маршруты трамваев: имя → остановки
	stopTrams  map[string]map[string]struct{} // Остановки → трамваи
}

func NewTramManager() *TramManager {
	return &TramManager{
		tramRoutes: make(map[string][]string),
		stopTrams:  make(map[string]map[string]struct{}),
	}
}

func (tm *TramManager) CreateTram(name string, stops []string) error {
	// Проверка на минимальное количество остановок
	if len(stops) < 2 {
		return errors.New("ERROR: At least two stops required")
	}

	// Проверка на повторяющиеся остановки
	uniqueStops := make(map[string]struct{})
	for _, stop := range stops {
		if _, exists := uniqueStops[stop]; exists {
			return errors.New("ERROR: Duplicate stops in route")
		}
		uniqueStops[stop] = struct{}{}
	}

	// Проверка на существующее имя
	if _, exists := tm.tramRoutes[name]; exists {
		return fmt.Errorf("ERROR: Tram '%s' already exists", name)
	}

	// Проверка на дубликат маршрута
	for existingName, existingRoute := range tm.tramRoutes {
		if slicesEqual(existingRoute, stops) {
			return fmt.Errorf("ERROR: Route duplicates existing tram '%s'", existingName)
		}
	}

	// Добавление нового маршрута
	tm.tramRoutes[name] = stops
	for _, stop := range stops {
		if tm.stopTrams[stop] == nil {
			tm.stopTrams[stop] = make(map[string]struct{})
		}
		tm.stopTrams[stop][name] = struct{}{}
	}

	return nil
}

func (tm *TramManager) GetTramsInStop(stop string) []string {
	trams, exists := tm.stopTrams[stop]
	if !exists {
		return nil
	}

	result := make([]string, 0, len(trams))
	for tram := range trams {
		result = append(result, tram)
	}
	sort.Strings(result)
	return result
}

func (tm *TramManager) GetStopsInTram(tram string) map[string][]string {
	stops, exists := tm.tramRoutes[tram]
	if !exists {
		return nil
	}

	result := make(map[string][]string)
	for _, stop := range stops {
		trams := make([]string, 0)
		for t := range tm.stopTrams[stop] {
			if t != tram {
				trams = append(trams, t)
			}
		}
		sort.Strings(trams)
		result[stop] = trams
	}

	return result
}

func (tm *TramManager) GetAllTrams() map[string][]string {
	return tm.tramRoutes
}

func slicesEqual(a, b []string) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}
