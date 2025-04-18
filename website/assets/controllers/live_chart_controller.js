import { Controller } from '@hotwired/stimulus';

export default class extends Controller {
    connect() {
        this.element.addEventListener('live:render', () => {
            if (this.chart) {
                this.chart.destroy();
            }
            const ctx = this.element.querySelector('canvas').getContext('2d');
            this.chart = new Chart(ctx, JSON.parse(this.element.dataset.chartData));
        });
    }
}
