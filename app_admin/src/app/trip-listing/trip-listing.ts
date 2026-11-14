
import { Component, ChangeDetectorRef } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { Trip } from '../trip';
import { TripCard } from '../trip-card/trip-card';
import { TripData } from '../trip-data';
import { TripForm } from '../trip-form/trip-form';

@Component({
  selector: 'app-trip-listing',
  standalone: true,
  imports: [CommonModule, FormsModule, TripCard, TripForm],
  templateUrl: './trip-listing.html',
  styleUrl: './trip-listing.css'
})
export class TripListing {
  trips: Trip[] = [];
  showForm = false;
  selectedTrip: Trip | null = null;

  searchTerm = '';

  successMessage = '';
  errorMessage = '';

  constructor(
    private tripData: TripData,
    private cdr: ChangeDetectorRef
  ) {}

  get filteredTrips(): Trip[] {
    const search = this.searchTerm.trim().toLowerCase();

    if (!search) {
      return this.trips;
    }

    return this.trips.filter(trip =>
      trip.name.toLowerCase().includes(search) ||
      trip.code.toLowerCase().includes(search) ||
      trip.resort.toLowerCase().includes(search)
    );
  }

  ngOnInit(): void {
    this.loadTrips();
  }

  loadTrips(): void {
    this.errorMessage = '';

    this.tripData.getTrips().subscribe({
      next: (data) => {
        this.trips = data;
        this.cdr.detectChanges();
      },
      error: (err) => {
        console.error('Error loading trips:', err);
        this.errorMessage =
          'Unable to load trips. Please try again.';
        this.cdr.detectChanges();
      }
    });
  }

  addTrip(): void {
    this.clearMessages();
    this.selectedTrip = null;
    this.showForm = true;
  }

  editTrip(trip: Trip): void {
    this.clearMessages();
    this.selectedTrip = { ...trip };
    this.showForm = true;
  }

  deleteTrip(trip: Trip): void {
    this.clearMessages();

    if (!confirm(`Delete ${trip.name}?`)) {
      return;
    }

    this.tripData.deleteTrip(trip.code).subscribe({
      next: () => {
        this.loadTrips();
        this.successMessage =
          `${trip.name} was deleted successfully.`;
        this.cdr.detectChanges();
      },
      error: (err) => {
        console.error('Error deleting trip:', err);
        this.errorMessage =
          `Unable to delete ${trip.name}. Please try again.`;
        this.cdr.detectChanges();
      }
    });
  }

  onTripSaved(): void {
    this.showForm = false;
    this.selectedTrip = null;
    this.loadTrips();
    this.successMessage = 'Trip saved successfully.';
  }

  onCancel(): void {
    this.showForm = false;
    this.selectedTrip = null;
  }

  clearMessages(): void {
    this.successMessage = '';
    this.errorMessage = '';
  }
}
