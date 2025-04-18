import { startStimulusApp } from '@symfony/stimulus-bundle';
import ChartRefreshController from './controllers/chart_refresh_controller.js';
import LiveChartController from './controllers/live_chart_controller.js';

const app = startStimulusApp();
// register any custom, 3rd party controllers here
// app.register('some_controller_name', SomeImportedController);
app.register('chart-refresh', ChartRefreshController);
app.register('live-chart', LiveChartController);
