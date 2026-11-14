import { Component, EventEmitter, Output } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { HttpClient } from '@angular/common/http';

@Component({
  selector: 'app-login',
  standalone: true,
  imports: [CommonModule, FormsModule],
  templateUrl: './login.html'
})
export class Login {
  email = '';
  password = '';
  errorMessage = '';

  @Output() loggedIn = new EventEmitter<void>();

  constructor(private http: HttpClient) {}

  onLogin(): void {
    this.errorMessage = '';

    this.http.post<{ token: string }>(
      'http://localhost:3000/api/login',
      { email: this.email, password: this.password }
    ).subscribe({
      next: (response) => {
        sessionStorage.setItem('travlr-token', response.token);
        this.loggedIn.emit();
      },
      error: () => {
        this.errorMessage = 'Invalid email or password.';
      }
    });
  }
}