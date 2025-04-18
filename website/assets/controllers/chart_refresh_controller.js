import { Controller } from '@hotwired/stimulus';

export default class extends Controller {
    static values = {
        interval: Number
    }

    connect() {
        this.refresh();
        this.timer = setInterval(() => this.refresh(), this.intervalValue || 5000);
    }

    disconnect() {
        clearInterval(this.timer);
    }

    refresh() {
        this.element.dispatchEvent(new CustomEvent('live:refresh', { bubbles: true }));
    }
}
