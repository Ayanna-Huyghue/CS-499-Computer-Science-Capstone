
import { Component } from '@angular/core';
import { CommonModule } from '@angular/common';
import { TripListing } from './trip-listing/trip-listing';
import { Login } from './login/login';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [CommonModule, TripListing, Login],
  templateUrl: './app.html',
  styleUrl: './app.css'
})
export class App {
  title = 'travlr-admin';

  isLoggedIn = !!sessionStorage.getItem('travlr-token');

  onLoggedIn(): void {
    this.isLoggedIn = true;
  }

  logout(): void {
    sessionStorage.removeItem('travlr-token');
    this.isLoggedIn = false;
  }
}
