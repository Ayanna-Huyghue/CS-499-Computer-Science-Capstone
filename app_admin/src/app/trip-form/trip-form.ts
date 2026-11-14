
import { Component, EventEmitter, Input, Output, OnChanges, SimpleChanges } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { Trip } from '../trip';
import { TripData } from '../trip-data';

@Component({
  selector: 'app-trip-form',
  standalone: true,
  imports: [CommonModule, FormsModule],
  templateUrl: './trip-form.html',
  styleUrl: './trip-form.css'
})
export class TripForm implements OnChanges {
  @Input() selectedTrip: Trip | null = null;
  @Output() saved = new EventEmitter<void>();
  @Output() cancelled = new EventEmitter<void>();

  trip: Trip = this.emptyTrip();
  isEditing = false;
  errorMessage = '';

  constructor(private tripData: TripData) {}

  ngOnChanges(changes: SimpleChanges): void {
    if (changes['selectedTrip']) {
      this.isEditing = !!this.selectedTrip;
      this.trip = this.selectedTrip
        ? { ...this.selectedTrip }
        : this.emptyTrip();
      this.errorMessage = '';
    }
  }

  private emptyTrip(): Trip {
    return {
      code: '',
      name: '',
      length: '',
      start: '',
      resort: '',
      perPerson: '',
      image: '',
      description: ''
    };
  }

  saveTrip(): void {
    this.errorMessage = '';

    const request = this.isEditing
      ? this.tripData.updateTrip(this.trip)
      : this.tripData.addTrip(this.trip);

    request.subscribe({
      next: () => this.saved.emit(),
      error: (err) => {
        console.error('Error saving trip:', err);
        this.errorMessage = 'Unable to save trip. Check the API and trip details.';
      }
    });
  }

  cancel(): void {
    this.cancelled.emit();
  }
}
