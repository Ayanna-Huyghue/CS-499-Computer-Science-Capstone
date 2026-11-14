
import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import { Trip } from './trip';

@Injectable({
  providedIn: 'root'
})
export class TripData {
  private apiUrl = 'http://localhost:3000/api/trips';

  constructor(private http: HttpClient) {}

  private getAuthHeaders(): HttpHeaders {
    const token = sessionStorage.getItem('travlr-token');

    return new HttpHeaders({
      Authorization: `Bearer ${token || ''}`
    });
  }

  getTrips(): Observable<Trip[]> {
    return this.http.get<Trip[]>(this.apiUrl);
  }

  addTrip(trip: Trip): Observable<Trip> {
    return this.http.post<Trip>(this.apiUrl, trip, {
      headers: this.getAuthHeaders()
    });
  }

  updateTrip(trip: Trip): Observable<Trip> {
    return this.http.put<Trip>(
      `${this.apiUrl}/${trip.code}`,
      trip,
      { headers: this.getAuthHeaders() }
    );
  }

  deleteTrip(code: string): Observable<any> {
    return this.http.delete(`${this.apiUrl}/${code}`, {
      headers: this.getAuthHeaders()
    });
  }
}
